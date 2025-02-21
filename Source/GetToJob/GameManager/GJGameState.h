#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GJGameState.generated.h"

UCLASS()
class GETTOJOB_API AGJGameState : public AGameState
{
	GENERATED_BODY()

public:
	AGJGameState();

	//점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	
	//최대 타임 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelLimitTime;

	//몬스터 킬 카운트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KillCount")
	int32 EnemyKillCount;

	//현재 레벨의 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentWaveIndex;

	//최대 레벨의 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	//레벨 저장
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	//timer
	FTimerHandle LevelTimerHandle;
	//ui
	FTimerHandle UIUpdateTimerHandle;

	//점수 가져오기
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	//점수 추가
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	//몬스터 킬 가져오기
	UFUNCTION(BlueprintPure, Category = "KillCount")
	int32 GetEnemyKillCount() const;

	//몬스터 킬 추가
	UFUNCTION(BlueprintCallable, Category = "KillCount")
	void AddEnemyKillCount(int32 Amount);

	//타이머가 돌아갈 BeginPlay 추가
	virtual void BeginPlay() override;

	//웨이브 시작 세팅
	void StartWave();

	//다음 웨이브로 넘어가는 함수
	void NextWave();

	//보스 레벨로 넘어가는 함수

	
};
