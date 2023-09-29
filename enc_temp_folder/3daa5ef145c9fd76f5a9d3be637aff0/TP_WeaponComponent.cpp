#include "TP_WeaponComponent.h"

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UTP_WeaponComponent::AttachWeapon(USceneComponent* WeaponSocket, FName SocketName)
{
	if (WeaponSocket)
	{
		FActorSpawnParameters ActorSpawnParams;
		FTransform SpawnTransform;
		SpawnTransform = Owner->GetTransform();
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (WeaponInHand == nullptr)
		{
			WeaponInHand = GetWorld()->SpawnActor<AAxeActor>(AxeActor, SpawnTransform, ActorSpawnParams);
			WeaponInHand->AttachToComponent(WeaponSocket, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			WeaponInHand->SetActorRelativeRotation(FRotator(90.0f, 200.0f, 108.0f));
			WeaponInHand->SetActorRelativeLocation(FVector(0.0f, -6.0f, -1.0f));
			////WeaponInHand->SetActorRelativeScale3D(FVector(0.5f, 0.5f, 0.7f));
		}
	}
}

void UTP_WeaponComponent::Attack(float BaseDamage)
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		FHitResult HitResult;
		TArray<AActor*> IgnoredActors{Owner};

		DrawTrace(HitResult, IgnoredActors);

		if (AActor* HitActor = Cast<AChoppableTree>(HitResult.GetActor()))
		{
			float ChopAnimationTime = ChopAnimation->GetPlayLength();
			AnimInstance->Montage_Play(ChopAnimation);
			FTimerHandle MontageDelayHandle;
			GetWorld()->GetTimerManager().SetTimer(MontageDelayHandle, [=]()
			{
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), 40.0f, nullptr, Owner, UDamageType::StaticClass());
			},
			ChopAnimationTime, false);
		}
	}
}

void UTP_WeaponComponent::DrawTrace(FHitResult& HitResult, const TArray<AActor*> IgnoredActors) const
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
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f);
}

void UTP_WeaponComponent::SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const
{
	StartLocation = Owner->GetActorLocation();
	EndLocation = StartLocation + Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() *
		MaxInteractiveDistance;
}

