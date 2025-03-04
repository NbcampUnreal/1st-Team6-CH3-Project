#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsBossRage.generated.h"


UCLASS()
class GETTOJOB_API UBTService_IsBossRage : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_IsBossRage();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "yes"))
	float LeftHp = 5000.f;
};
