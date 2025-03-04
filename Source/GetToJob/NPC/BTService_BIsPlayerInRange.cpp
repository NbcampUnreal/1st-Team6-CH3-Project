#include "NPC/BTService_BIsPlayerInRange.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTService_BIsPlayerInRange::UBTService_BIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UBTService_BIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
					npc->GetDistanceTo(player) <= Range);
				const float RandValue = FMath::FRandRange(0.0f, 100.f);
				if (npc->GetDistanceTo(player) <= Range)
				{
					if (RandValue <= 1.f)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCanRangeAttack"), true);
					}
				}
			}
		}
	}
}