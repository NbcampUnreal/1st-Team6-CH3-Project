#include "NPC/BTTask_IncrementPathIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GJ_AIController.h"
#include "GJNPC.h"
#include "PatrolPath.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try and get the AI controller
	if (auto* const cont = Cast<AGJ_AIController>(OwnerComp.GetAIOwner()))
	{
		// try and get the NPC
		if (auto* const NPC = Cast<AGJNPC>(cont->GetPawn()))
		{
			// try and get the blackboard
			if (auto* const bc = OwnerComp.GetBlackboardComponent())
			{
				APatrolPath* PatrolPath = NPC->GetPatrolParth();
				if (PatrolPath == nullptr) // Null üũ!
				{
					UE_LOG(LogTemp, Warning, TEXT("PatrolPath is null!"));
					return EBTNodeResult::Failed;
				}

				// get number of patrol points and min and max indices
				auto const NoOfPoints = NPC->GetPatrolParth()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = bc->GetValueAsInt(GetSelectedBlackboardKey());

				// change direction if we are at the first or last index if we are in bidirectional mode
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				//write new value of index to blackboard
				bc->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

