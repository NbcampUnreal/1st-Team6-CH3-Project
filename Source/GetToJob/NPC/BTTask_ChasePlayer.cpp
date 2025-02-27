#include "NPC/BTTask_ChasePlayer.h"
#include "GJ_AIController.h"
#include "NPC/GJBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get target location from blackboard via the NPC'S Controller
	if (auto* const cont = Cast<AGJ_AIController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the Player's Location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		//finish with Success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	if (auto* const cont = Cast<AGJBossAIController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the Player's Location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		//finish with Success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}