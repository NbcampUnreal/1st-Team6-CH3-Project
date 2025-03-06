#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GJGameState.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class GETTOJOB_API AGJGameState : public AGameState
{
	GENERATED_BODY()

public:
	AGJGameState();

	//����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	
	//�ִ� Ÿ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelLimitTime;

	//���� ų ī��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KillCount")
	int32 EnemyKillCount;

	//���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentWaveIndex;

	//�ִ� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	//timer
	FTimerHandle LevelTimerHandle;
	//ui
	FTimerHandle UIUpdateTimerHandle;

	//���� ��������
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	//���� �߰�
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	//���� ų ��������
	UFUNCTION(BlueprintPure, Category = "KillCount")
	int32 GetEnemyKillCount() const;

	//���� ų �߰�
	UFUNCTION(BlueprintCallable, Category = "KillCount")
	void AddEnemyKillCount(int32 Amount);

	//Ÿ�̸Ӱ� ���ư� BeginPlay �߰�
	virtual void BeginPlay() override;

	//���̺� ���� ����
	void StartWave();

	//���� ���̺�� �Ѿ�� �Լ�
	void NextWave();

	//���� ������ �Ѿ�� �Լ�

	void LoadNextLevel();

	FTimerHandle WaveTransitionTimerHandle; // 유예시간 타이머 핸들 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* MainBGM;

	UAudioComponent* AudioComponent;

	void PlaySound();
	void StopSound();
};
