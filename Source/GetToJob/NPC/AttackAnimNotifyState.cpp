#include "NPC/AttackAnimNotifyState.h"
#include "AICharacterBase.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			if (AGJBossNPC* const BossNPC = Cast<AGJBossNPC>(MeshComp->GetOwner()))
			{
				BossNPC->AttackStart();
				if (AGJBossAIController* const Boss = Cast<AGJBossAIController>(character->GetController()))
				{
					if (UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Boss->GetBrainComponent()))
					{
						if (Compo->GetBlackboardComponent()->GetValueAsBool(TEXT("IsCanRangeAttack")))
						{
							UCharacterMovementComponent* MovementComponent = BossNPC->GetCharacterMovement();
							if (MovementComponent)
							{
								MovementComponent->SetMovementMode(MOVE_None);
							}
						}
					}
				}
			}
			else character->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AAICharacterBase* const character = Cast<AAICharacterBase>(MeshComp->GetOwner()))
		{
			if (AGJBossNPC* const BossNPC = Cast<AGJBossNPC>(MeshComp->GetOwner()))
			{
				BossNPC->AttackEnd();
				if (AGJBossAIController* const Boss = Cast<AGJBossAIController>(character->GetController()))
				{
					if (UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Boss->GetBrainComponent()))
					{
						if (Compo->GetBlackboardComponent()->GetValueAsBool(TEXT("IsCanRangeAttack")))
						{
							UCharacterMovementComponent* MovementComponent = BossNPC->GetCharacterMovement();
							if (MovementComponent)
							{
								MovementComponent->SetMovementMode(MOVE_Walking);
							}
						}
					}
				}
			}
			else character->AttackEnd();
			if (AGJBossAIController* const Boss = Cast<AGJBossAIController>(character->GetController()))
			{
				if (UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Boss->GetBrainComponent()))
				{
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
					Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCanRangeAttack"), false);
				}
			}
		}
	}
}

