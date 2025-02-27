#include "NPC/BTService_BIsPlayerInMeleeRange.h"
#include "BTService_BIsPlayerInMeleeRange.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_BIsPlayerInMeleeRange::UBTService_BIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_BIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller and NPC
	if (auto const* const cont = Cast<AGJBossAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto const* const npc = Cast<AGJBossNPC>(cont->GetPawn()))
		{
			// get Player Character
			if (auto const* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				// write true or false to the blackboard key depending on whether or not the player is in range
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(
					GetSelectedBlackboardKey(),
					npc->GetDistanceTo(player) <= MeleeRange);
				if (npc->GetDistanceTo(player) <= MeleeRange)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), true);
				}
			}
		}
	}
}
