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
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		//finish the task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	// npc's are in range so get the AI's Controller and the NPC itself
	auto const* const cont = OwnerComp.GetAIOwner();
	auto* const npc = Cast<AGJBossNPC>(cont->GetPawn());
	const float RandValue = FMath::FRandRange(0.0f, 3.f);
	if (RandValue <= 1.0)
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{

			// necessary check to see if the montage has finished so we don't try and play it again
			if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc))
			{
				icombat->Execute_WeakAttack(npc);
				npc->SetIsFist(true);
			}

		}
	}
	else if (RandValue > 1.0 && RandValue <= 2.0)
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{
			// necessary check to see if the montage has finished so we don't try and play it again

			if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc))
			{
				icombat->Execute_StrongAttack(npc);
				npc->SetIsFist(false);
			}
		}
	}
	else
	{
		// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
		if (auto* const icombat = Cast<ICombatInterface>(npc))
		{
			// necessary check to see if the montage has finished so we don't try and play it again
			if (SpecialMontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc))
			{
				icombat->Execute_SpecialAttack(npc);
				npc->SetIsFist(false);
			}
		}
	}
	// finish the task
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
