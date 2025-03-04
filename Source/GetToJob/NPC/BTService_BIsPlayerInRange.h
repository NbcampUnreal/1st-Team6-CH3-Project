#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BIsPlayerInRange.generated.h"


UCLASS()
class GETTOJOB_API UBTService_BIsPlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_BIsPlayerInRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "yes"))
	float Range = 1200.f;
};
