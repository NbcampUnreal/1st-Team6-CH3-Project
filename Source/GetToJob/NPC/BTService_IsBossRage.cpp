#include "NPC/BTService_IsBossRage.h"
#include "NPC/GJBossAIController.h"
#include "NPC/GJBossNPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsBossRage::UBTService_IsBossRage()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Hp Detect");
}

void UBTService_IsBossRage::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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
					npc->GetHealth() <= LeftHp);
			}
		}
	}
}
