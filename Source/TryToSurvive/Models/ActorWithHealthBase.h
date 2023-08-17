#pragma once

#include "ActorWithHealthBase.generated.h"

UCLASS(ClassGroup=(Custom))
class AActorWithHealthBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Property")
	float Health;

	virtual void ReceiveDamage(float damage);
};
