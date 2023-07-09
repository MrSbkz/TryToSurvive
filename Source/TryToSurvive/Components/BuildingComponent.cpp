#include "BuildingComponent.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TryToSurvive/TryToSurviveCharacter.h"

UTTS_BuildingComponent::UTTS_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTTS_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ATryToSurviveCharacter>(GetOwner());
}

void UTTS_BuildingComponent::ResetBuilding()
{
	CurrentBuildItem->Destroy();
	CurrentBuildItem = nullptr;
}

void UTTS_BuildingComponent::Build()
{
	if (!IsBuildingEnable) return;

	IsBuildingMode = false;

	// CurrentBuildItem->SetActorEnableCollision(true);
	CurrentBuildItem->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetBuildingMaterial(EBuildingMaterialType::Base);

	CurrentBuildItem = nullptr;
	CurrentMaterial = nullptr;
}

void UTTS_BuildingComponent::SetPlayerController(APlayerController* PlayerController)
{
	CharacterPlayerController = PlayerController;
}

void UTTS_BuildingComponent::ProcessRotationMode()
{
	if (IsBuildingMode)
	{
		FRotator BuildingRotation = CurrentBuildItem->GetActorRotation();
		BuildingRotation.Yaw += 30.0f;
		CurrentBuildItem->SetActorRotation(BuildingRotation);
	}
}

void UTTS_BuildingComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterPlayerController && Owner)
	{
		if (IsBuildingMode)
		{
			FHitResult HitResult;
			TArray<AActor*> IgnoredActors{Owner};
			if (CurrentBuildItem)
			{
				IgnoredActors.Add(CurrentBuildItem);
			}
			DrawTrace(HitResult, IgnoredActors);

			StartPreview(HitResult);

			// const FRotator SpawnRotation = CharacterPlayerController->PlayerCameraManager->GetCameraRotation();
			// const FVector PlayerLocation = Owner->GetActorLocation();
			//
			// // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			// BuildingSpawnLocation = PlayerLocation + SpawnRotation.RotateVector(BuildItemOffset);
			//
			// // TODO: Replace this, z location won't be always 0
			// BuildingSpawnLocation.Z = 0.0f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Owner is nullptr"), __FUNCTION__);
	}

	if (IsRotating)
	{
		FRotator BuildingRotation = CurrentBuildItem->GetActorRotation();
		BuildingRotation.Yaw += 30.0f;
		CurrentBuildItem->SetActorRotation(BuildingRotation);
	}
}

void UTTS_BuildingComponent::StartPreview(const FHitResult& HitResult)
{
	if (CurrentBuildItem)
	{
		SetBuildingLocation(HitResult);
	}
	else
	{
		CreateBuildingItem(EBuildingMaterialType::Preview, HitResult);
		//CurrentBuildItem->SetActorEnableCollision(false);
		CurrentBuildItem->MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UTTS_BuildingComponent::SetBuildingLocation(const FHitResult& HitResult)
{
	FVector BuildingLocation;
	if (HitResult.bBlockingHit)
	{
		BuildingLocation = HitResult.Location.GridSnap(GridSnapValue);
		if (CurrentBuildItem->IsBuildingEnable && HitResult.GetActor()->GetClass()->GetFName() != "BP_Wall_C")
		{
			FName name = HitResult.GetActor()->GetClass()->GetFName();
			UE_LOG(LogTemp, Error, TEXT("[%S] Hit result name: %s"), __FUNCTION__, *name.ToString());
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "Building is enable");
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Building is disable");
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Building is disable");
		FVector StartLocation(ForceInitToZero), EndLocation(ForceInitToZero);
		SetStartEndLocation(StartLocation, EndLocation);

		BuildingLocation = EndLocation;
	}

	const TArray<AActor*> IgnoredActors = {CurrentBuildItem};
	TArray<FHitResult> BoxHits;
	FVector BoxLocation = BuildingLocation + CurrentBuildingExtend;
	BoxLocation.Y = BoxLocation.Y - CurrentBuildingExtend.Y;
	BoxLocation.X = BoxLocation.X - CurrentBuildingExtend.X;

	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		BoxLocation,
		BoxLocation,
		CurrentBuildingExtend,
		CurrentBuildItem->GetActorRotation(),
		TraceTypeQuery8,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		BoxHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f
	);

	for (const auto& Hit : BoxHits)
	{
		BuildingLocation += Hit.Normal;
	}

	CurrentBuildItem->SetActorLocation(BuildingLocation);

	IsBuildingEnable = IsBuildingOnGround();
}

void UTTS_BuildingComponent::CreateBuildingItem(const EBuildingMaterialType MaterialType, const FHitResult& HitResult)
{
	FTransform Transform;
	Transform.SetLocation(HitResult.Location);
	CurrentBuildItem = GetWorld()->SpawnActor<ABuildingActorBase>(BuildingItems[0], Transform);

	if (CurrentBuildItem)
	{
		FVector Origin(ForceInitToZero), Extend(ForceInitToZero);
		CurrentBuildItem->GetActorBounds(false, Origin, Extend);
		CurrentBuildingExtend = Extend;
	}

	SetBuildingMaterial(MaterialType);
}

void UTTS_BuildingComponent::SetBuildingMaterial(const EBuildingMaterialType MaterialType)
{
	if (CurrentBuildItem && GetWorld() && CurrentBuildItem->PreviewMaterial && CurrentBuildItem->BaseMaterial)
	{
		switch (MaterialType)
		{
		case EBuildingMaterialType::Preview:
			CurrentMaterial = UMaterialInstanceDynamic::Create(CurrentBuildItem->PreviewMaterial, GetWorld());
			break;
		case EBuildingMaterialType::Base:
			CurrentMaterial = UMaterialInstanceDynamic::Create(CurrentBuildItem->BaseMaterial, GetWorld());
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("[%S] Material type has not been set"), __FUNCTION__);
			break;
		}

		CurrentBuildItem->MeshComponent->SetMaterial(0, CurrentMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Material has not been set"), __FUNCTION__);
	}
}

void UTTS_BuildingComponent::DrawTrace(FHitResult& HitResult, const TArray<AActor*> IgnoredActors)
{
	FVector StartLocation(ForceInitToZero), EndLocation(ForceInitToZero);
	SetStartEndLocation(StartLocation, EndLocation);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f);
}

void UTTS_BuildingComponent::SetStartEndLocation(FVector& StartLocation, FVector& EndLocation)
{
	StartLocation = Owner->GetActorLocation();
	EndLocation = StartLocation + Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() *
		MaxBuildingDistance;
}

bool UTTS_BuildingComponent::IsBuildingOnGround()
{
	FHitResult OutHit;
	FVector Start = CurrentBuildItem->GetActorLocation();
	FVector End = Start + FVector(0.f, 0.f, -1.f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;

	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
}
