#include "ChoppableTree.h"

AChoppableTree::AChoppableTree()
{
	Health = 100.0f;
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	FVector Location = Mesh->GetComponentLocation();

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionBox"));
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CapsuleComponent->SetRelativeLocation(FVector(Location.X, Location.Y, 150.0f));
	CapsuleComponent->SetCapsuleSize(48.0f, 300.0f);
	CapsuleComponent->SetupAttachment(Mesh);

	OnTakeAnyDamage.AddDynamic(this, &AChoppableTree::OnAnyDamage);
}

void AChoppableTree::BeginPlay()
{
	Super::BeginPlay();
	
	SetNumberOfLogs();
}

void AChoppableTree::SetNumberOfLogs()
{
	AmountOfLogs = FMath::RandRange(4, 6);
}

void AChoppableTree::ReceiveDamage(float damage)
{
	Super::ReceiveDamage(damage);

	if ( (Health - damage) <= 0)
	{
	    SpawnLocation = GetActorLocation();
	    SpawnTransform = GetActorTransform();
		
		SpawnStump();

		K2_DestroyActor();

		SpawnDrop();
	}
}

void AChoppableTree::SpawnStump()
{
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AStumpActor>(StumpActor, SpawnTransform, ActorSpawnParams);
}

void AChoppableTree::SpawnLog()
{
	SpawnLocation.X += 30.0f;
	SpawnLocation.Z += 30.0f;
	
	GetWorld()->SpawnActor<ALogActor>(LogActor, SpawnLocation, FRotator(90.0f, 0.0f, 0.0f), ActorSpawnParams);
}

void AChoppableTree::SpawnDrop()
{
	for(int i = 0; i < AmountOfLogs; i++)
	{
		SpawnLog();
	}
}

void AChoppableTree::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	ReceiveDamage(Damage);
}

