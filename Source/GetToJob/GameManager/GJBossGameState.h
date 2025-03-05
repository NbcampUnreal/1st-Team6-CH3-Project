#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GJBossGameState.generated.h"

class USoundBase;
class UAudioComponent;
class AHUD;

UCLASS()
class GETTOJOB_API AGJBossGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AGJBossGameState();

	int32 Score;
	int32 GetScore();
	void AddScore(int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float BossLimitTime;
	FTimerHandle LevelTimerHandle;

	int32 EnemyKillCount;
	int32 GetEnemyKillCount();
	void AddEnemyKillCount(int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BossBGM;

	UAudioComponent* AudioComponent;

	void PlaySound();
	void StopSound();

	FTimerHandle UIUpdateTimerHandle;

	virtual void BeginPlay() override;

	bool bIsBossDefeated = false;

	void BossStart();
	void BossFail();

	void SetBossDefeated();


};
