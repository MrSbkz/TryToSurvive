#include "BuildingActorBase.h"

ABuildingActorBase::ABuildingActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	
	// BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	// BoxComponent->SetupAttachment(MeshComponent);
	//
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildingActorBase::OnOverlapBegin);
	// BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildingActorBase::OnOverlapEnd);
}

void ABuildingActorBase::BeginPlay()
{
	Super::BeginPlay();

	
	// FVector Origin(ForceInitToZero), Extend(ForceInitToZero);
	// GetActorBounds(false, Origin, Extend);
	//
	// BoxComponent->SetBoxExtent(Extend);
}

void ABuildingActorBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	TArray<UObject*> Subobjects;
	OtherActor->GetSubobjectsWithStableNamesForNetworking(Subobjects);
	for(const auto obj : Subobjects)
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] obj: %s"), __FUNCTION__, *obj->GetName());
		
	}
	IsBuildingEnable = false;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "OnOverlapBegin");
	UE_LOG(LogTemp, Error, TEXT("[%S] OtherActor: %s"), __FUNCTION__, *OtherActor->GetFName().ToString());
}

void ABuildingActorBase::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	IsBuildingEnable = true;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, "OnOverlapEnd");
}


