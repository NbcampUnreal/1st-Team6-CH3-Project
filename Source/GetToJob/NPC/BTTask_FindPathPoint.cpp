#include "BTTask_FindPathPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GJ_AIController.h"
#include "GJNPC.h"
#include "PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// attemp to get the NPC's Controller
	if (auto* const cont = Cast<AGJ_AIController>(OwnerComp.GetAIOwner()))
	{
		// attemp to get the blackboard component from the behavior Tree
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			// get the current patrol path index from the blackboard
			auto const Index = bc->GetValueAsInt((GetSelectedBlackboardKey()));
			// get the NPC
			if (auto* npc = Cast<AGJNPC>(cont->GetPawn()))
			{
				APatrolPath* PatrolPath = npc->GetPatrolParth();
				if (PatrolPath == nullptr)
				{
					return EBTNodeResult::Failed;
				}
				// get the current patrol path vector from the NPC - this is local to the patrol path actor
				auto const Point = npc->GetPatrolParth()->GetPatrolPoint(Index);

				// convert the local vector to a global point
				auto const GlobalPoint = npc->GetPatrolParth()->GetActorTransform().TransformPosition(Point);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}