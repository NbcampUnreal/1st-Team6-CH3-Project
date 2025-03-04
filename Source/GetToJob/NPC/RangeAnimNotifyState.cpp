#include "NPC/RangeAnimNotifyState.h"
#include "AICharacterBase.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "GameFramework/CharacterMovementComponent.h"

void URangeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void URangeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			if (AGJBossNPC* const BossNPC = Cast<AGJBossNPC>(MeshComp->GetOwner()))
			{
				BossNPC->FireProjectile();
			}
		}
	}
}

