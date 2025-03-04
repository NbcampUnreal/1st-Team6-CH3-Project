#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "NPC/GJBossNPC.h"
#include "BTTask_BossRage.generated.h"

UCLASS()
class GETTOJOB_API UBTTask_BossRage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BossRage(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool MontageHasFinished(AGJBossNPC* const npc);

};
