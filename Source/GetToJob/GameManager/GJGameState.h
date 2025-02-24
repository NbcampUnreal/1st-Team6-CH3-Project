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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;


	FTimerHandle LevelTimerHandle;

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

	
};
