#include "ChoppableTree.h"

AChoppableTree::AChoppableTree()
{
	Health = 100.0f;
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName(TEXT("Overlap")); 
	CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 54.0f));
	CollisionBox->SetBoxExtent(FVector(132.0f, 132.0f, 48.0f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AChoppableTree::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AChoppableTree::OnEndOverlap);
	CollisionBox->SetupAttachment(Mesh);

	OnTakeAnyDamage.AddDynamic(this, &AChoppableTree::OnAnyDamage);
}

void AChoppableTree::BeginPlay()
{
	Super::BeginPlay();
	
	if (Cast<ATryToSurviveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		CharacterRef = Cast<ATryToSurviveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		SetNumberOfLogs();
	}
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

void AChoppableTree::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::IsValid(CharacterRef) && OtherActor == CharacterRef)
	{
		CharacterRef->AttackState = EAttackState::Gather;
		CharacterRef->HarvestActor.Add(this);
	}
}

void AChoppableTree::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UKismetSystemLibrary::IsValid(CharacterRef) && OtherActor == CharacterRef)
	{
		CharacterRef->AttackState = EAttackState::Attack;
		CharacterRef->HarvestActor.Remove(this);
	}
}

void AChoppableTree::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (UKismetSystemLibrary::IsValid(CharacterRef))
	{
		ReceiveDamage(Damage);
	}
}

