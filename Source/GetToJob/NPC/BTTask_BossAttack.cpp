#include "NPC/BTTask_BossAttack.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LatentActionManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NPC/CombatInterface.h"
#include "NPC/GJBossAIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"


UBTTask_BossAttack::UBTTask_BossAttack()
{
	NodeName = "Boss Attack";
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if npc are out of range, do not attack the player
	auto const* const cont = OwnerComp.GetAIOwner();
	auto* const npc = Cast<AGJBossNPC>(cont->GetPawn());
	auto* const Controller = Cast<AGJBossAIController>(npc->GetController());
	UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent());
	bool IsRage = Compo->GetBlackboardComponent()->GetValueAsBool(TEXT("IsBossDoRage"));
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		//finish the task
		Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	// npc's are in range so get the AI's Controller and the NPC itself

	const float RandValue = FMath::FRandRange(0.0f, 9.f);
	if (RandValue <= 4.0f)
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{
			if (!IsRage) 
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_WeakAttack(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}
			else
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_WeakAttack_Rage(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}

		}
	}
	else if (RandValue > 4.0f && RandValue <= 7.0f)
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{
			// necessary check to see if the montage has finished so we don't try and play it again
			if (!IsRage)
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_StrongAttack(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}
			else
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_StrongAttack_Rage(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}
		}
	}
	else if (RandValue > 7.0f && RandValue <= 9.0f)
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{
			if (!IsRage)
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_SpecialAttack(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}
			else
			{
				// necessary check to see if the montage has finished so we don't try and play it again
				if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && RangeMontageHasFinished(npc))
				{
					icombat->Execute_SpecialAttack_Rage(npc);
					npc->SetIsFist(true);
					npc->SetIsRight(false);
					npc->SetIsLeft(false);
				}
			}
		}
	}
	// finish the task
	Compo->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_BossAttack::WeakMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetWeakMontage());
}

bool UBTTask_BossAttack::StrongMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetStrongMontage());
}

bool UBTTask_BossAttack::SpecialMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetSpecialMontage());
}

bool UBTTask_BossAttack::RangeMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetRangeMontage());
}
