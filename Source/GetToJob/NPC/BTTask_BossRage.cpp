#include "NPC/BTTask_BossRage.h"
#include "CombatInterface.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UBTTask_BossRage::UBTTask_BossRage(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Boss Rage");
}

EBTNodeResult::Type UBTTask_BossRage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// npc's are in range so get the AI's Controller and the NPC itself
	auto const* const cont = OwnerComp.GetAIOwner();
	auto* const npc = Cast<AGJBossNPC>(cont->GetPawn());


	// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
	if (auto* const icombat = Cast<ICombatInterface>(npc))
	{
		// necessary check to see if the montage has finished so we don't try and play it again
		if (MontageHasFinished(npc))
		{
			icombat->Execute_RageMotion(npc);
		}
	}

	// finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_BossRage::MontageHasFinished(AGJBossNPC* const npc)
{
	if (npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetRageMontage()))
		return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetRageMontage());
	else return false;
}
