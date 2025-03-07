#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GJBossGameMode.generated.h"

class AGJHUD;

UCLASS()
class GETTOJOB_API AGJBossGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AGJBossGameMode();

protected:
	virtual void BeginPlay() override;

private:
	AGJHUD* HUD;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClearScore")
	int32 ClearScoreCheck;

	void OnBossDefeated();

	void StartCinematic();

	void OnCinematicFinished();

	bool ResultCheck = false;
};
