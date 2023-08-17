#include "TP_WeaponComponent.h"

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UTP_WeaponComponent::Attack(AActor* DamagedActor, float BaseDamage, EAttackState AttackState)
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	switch (AttackState)
	{
		case EAttackState::Gather:
			if (AnimInstance)
			{
				float ChopAnimationTime = ChopAnimation->GetPlayLength();
				AnimInstance->Montage_Play(ChopAnimation);
				FTimerHandle MontageDelayHandle;
				GetWorld()->GetTimerManager().SetTimer(MontageDelayHandle, [=]()
				{
					UGameplayStatics::ApplyDamage(DamagedActor, 40.0f, nullptr, Owner, UDamageType::StaticClass());
				},
				ChopAnimationTime, false);
			}
			break;
		case EAttackState::Attack:
			break;
	}
}