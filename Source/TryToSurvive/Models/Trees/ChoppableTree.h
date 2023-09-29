#pragma once

#include <string>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
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

	UPROPERTY(EditDefaultsOnly, Category="Tree")
	TSubclassOf<class AStumpActor> StumpActor;

	UPROPERTY(EditDefaultsOnly, Category="Tree")
	TSubclassOf<class ALogActor> LogActor;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OverlapCollision)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh)
	UStaticMeshComponent* Mesh;

private:
	float AmountOfLogs;

	void SpawnLog();
	void SpawnDrop();
	void SpawnStump();
	void SetNumberOfLogs();
	virtual void ReceiveDamage(float damage) override;
};
