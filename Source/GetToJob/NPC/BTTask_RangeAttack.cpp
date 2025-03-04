#include "NPC/BTTask_RangeAttack.h"
#include "CombatInterface.h"
#include "AIController.h"
#include "GJBossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	NodeName = TEXT("Range Attack");
}

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	auto* const Controller = Cast<AGJBossAIController>(npc->GetController());
	UBehaviorTreeComponent* Compo = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent());
	bool IsRage = Compo->GetBlackboardComponent()->GetValueAsBool(TEXT("IsBossDoRage"));
	// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
	if (auto* const icombat = Cast<ICombatInterface>(npc))
	{
		// necessary check to see if the montage has finished so we don't try and play it again
		if (!IsRage) 
		{
			if (MontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && SpecialMontageHasFinished(npc))
			{
				icombat->Execute_RangeAttack(npc);
			}
		}
		else
		{
			if (MontageHasFinished(npc) && WeakMontageHasFinished(npc) && StrongMontageHasFinished(npc) && SpecialMontageHasFinished(npc))
			{
				icombat->Execute_RangeAttack_Rage(npc);
			}
		}
	}

	// finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_RangeAttack::MontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetRangeMontage());
}

bool UBTTask_RangeAttack::WeakMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetWeakMontage());
}

bool UBTTask_RangeAttack::StrongMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetStrongMontage());
}

bool UBTTask_RangeAttack::SpecialMontageHasFinished(AGJBossNPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetSpecialMontage());
}