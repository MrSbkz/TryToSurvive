#pragma once

#include <string>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <TryToSurvive/TryToSurviveCharacter.h>
#include <TryToSurvive/Models/Trees/LogActor.h>
#include <TryToSurvive/Models/Trees/StumpActor.h>
#include <TryToSurvive/Models/ActorWithHealthBase.h>
#include "ChoppableTree.generated.h"

UCLASS(ClassGroup=(Custom))
class TRYTOSURVIVE_API AChoppableTree : public AActorWithHealthBase
{
	GENERATED_BODY()
	
public:	
	AChoppableTree();

	FVector SpawnLocation;
	FTransform SpawnTransform;
	FActorSpawnParameters ActorSpawnParams;
	ATryToSurviveCharacter* CharacterRef;

	UPROPERTY(EditDefaultsOnly, Category="Tree")
	TSubclassOf<class AStumpActor> StumpActor;

	UPROPERTY(EditDefaultsOnly, Category="Tree")
	TSubclassOf<class ALogActor> LogActor;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OverlapCollision)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh)
	UStaticMeshComponent* Mesh;

private:
	float AmountOfLogs;

	void SpawnLog();
	void SpawnDrop();
	void SpawnStump();
	void SetNumberOfLogs();
	void ReceiveDamage(float damage);
};
