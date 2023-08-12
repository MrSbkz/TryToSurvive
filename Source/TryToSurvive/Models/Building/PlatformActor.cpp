#include "PlatformActor.h"

void APlatformActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();
	FVector BoxExtent = Extent;
	BoxExtent.Z -= 30.0f;
	MeshComponent->BoxComponent->SetBoxExtent(BoxExtent);
	MeshComponent->BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Z - 30.0f));
	MeshComponent->BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
}

bool APlatformActor::CanBuild()
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
