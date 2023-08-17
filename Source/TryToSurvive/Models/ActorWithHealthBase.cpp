#include "ActorWithHealthBase.h"

void AActorWithHealthBase::ReceiveDamage(float damage)
{
	if ( (Health - damage) > 0)
	{
		Health -= damage;
	}
}
