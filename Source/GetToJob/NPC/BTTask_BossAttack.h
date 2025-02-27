#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "NPC/GJBossNPC.h"
#include "BTTask_BossAttack.generated.h"

UCLASS()
class GETTOJOB_API UBTTask_BossAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BossAttack();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool WeakMontageHasFinished(AGJBossNPC* const npc);
	bool StrongMontageHasFinished(AGJBossNPC* const npc);
	bool SpecialMontageHasFinished(AGJBossNPC* const npc);
};
