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
	explicit AGJ_AIController(FObjectInitializer const& FObjectInitializer); // AI 의 인지능력을 담당할 Perception 모듈 Setup

protected:
	virtual void OnPossess(APawn* InPawn) override; // OnPossess 상속 받고 BehaviorTree 바인드

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	virtual void BeginPlay() override;
};
