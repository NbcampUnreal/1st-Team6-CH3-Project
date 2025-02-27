#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GJBossAIController.generated.h"


UCLASS()
class GETTOJOB_API AGJBossAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGJBossAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus); //#include "Perception/AIPerceptionTypes.h" ÇÊ¿ä

	virtual void BeginPlay() override;
};
