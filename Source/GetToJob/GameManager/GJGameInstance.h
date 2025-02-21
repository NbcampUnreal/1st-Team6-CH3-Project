#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GJGameInstance.generated.h"

UCLASS()
class GETTOJOB_API UGJGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGJGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 EnemyTotalKillCount; //전체 적 킬 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore; //최종 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 OneWaveScore; //첫번째 웨이브 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TwoWaveScore; //두번째 웨이브 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 ThreeWaveScore; //세번째 웨이브 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 BossWaveScore; //보스 웨이브 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;

	//점수 추가 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int amount);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SaveOneWaveScore(int amount);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SaveTwoWaveScore(int amount);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SaveThreeWaveScore(int amount);

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SaveBossScore(int amount);

	//전체 적 킬 수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToEnemyKill(int amount);

	//점수와 적킬 수 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetTotalScore();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetOneWaveScore();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetTwoWaveScore();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetThreeWaveScore();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetBossScore();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetEnemyTotalKillCount();
};
