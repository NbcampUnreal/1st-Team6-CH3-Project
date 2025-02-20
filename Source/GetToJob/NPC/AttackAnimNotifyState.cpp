#include "NPC/AttackAnimNotifyState.h"

#include"AICharacterBase.h"
void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackEnd();
		}
	}
}

