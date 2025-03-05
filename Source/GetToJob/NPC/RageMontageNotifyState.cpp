#include "NPC/RageMontageNotifyState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "AICharacterBase.h"

void URageMontageNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			if (AGJBossAIController* const Boss = Cast<AGJBossAIController>(character->GetController()))
			{
				if (UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Boss->GetBrainComponent()))
				{
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsBossDoRage"), true);
				}
			}
		}
		if (AGJBossNPC* const BossNPC = Cast<AGJBossNPC>(MeshComp->GetOwner()))
		{
			UCharacterMovementComponent* MovementComponent = BossNPC->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->SetMovementMode(MOVE_None);
			}
		}
	}
}

void URageMontageNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			if (AGJBossAIController* const Boss = Cast<AGJBossAIController>(character->GetController()))
			{
				if (UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Boss->GetBrainComponent()))
				{
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsMeleeRange"), false);
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCanRangeAttack"), false);
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
				}
			}
		}
		if (AGJBossNPC* const BossNPC = Cast<AGJBossNPC>(MeshComp->GetOwner()))
		{
			UCharacterMovementComponent* MovementComponent = BossNPC->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->SetMovementMode(MOVE_Walking);
			}
		}
	}

}
