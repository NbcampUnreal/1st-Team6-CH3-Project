#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GJ_AIController.generated.h"


UCLASS()
class GETTOJOB_API AGJ_AIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit AGJ_AIController(FObjectInitializer const& FObjectInitializer); // AI �� �����ɷ��� ����� Perception ��� Setup

protected:
	virtual void OnPossess(APawn* InPawn) override; // OnPossess ��� �ް� BehaviorTree ���ε�

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	virtual void BeginPlay() override;
};
