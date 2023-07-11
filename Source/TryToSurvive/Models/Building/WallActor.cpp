#include "WallActor.h"
#include "Kismet/KismetSystemLibrary.h"

void AWallActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();
	FVector BoxExtent = Extent;
	BoxExtent.Y -= 10.0f;
	BoxComponent->SetBoxExtent(BoxExtent);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Z));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
}

bool AWallActor::SetLocation(FVector& BuildingLocation)
{
	FVector Origin(ForceInitToZero), Extend(ForceInitToZero);
	GetActorBounds(false, Origin, Extend);
	const FVector CurrentBuildingExtend = Extend;

	const TArray<AActor*> IgnoredActors = {this};
	TArray<FHitResult> BoxHits;
	FVector BoxLocation = BuildingLocation + CurrentBuildingExtend;
	BoxLocation.Y = BoxLocation.Y - CurrentBuildingExtend.Y;
	BoxLocation.X = BoxLocation.X - CurrentBuildingExtend.X;

	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		BoxLocation,
		BoxLocation,
		CurrentBuildingExtend,
		GetActorRotation(),
		TraceTypeQuery8,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
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

	SetActorLocation(BuildingLocation);
	
	return CanBuild();
}

bool AWallActor::CanBuild()
{
	FHitResult OutHit;
	FVector Start = GetActorLocation();
	FVector DownVector = Start + FVector(0.0f, 0.0f, -1.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;

	if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, DownVector, ECC_Visibility, CollisionParams))
	{
		return false;
	}

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, nullptr);

	return OverlappingActors.Num() == 0;
}
