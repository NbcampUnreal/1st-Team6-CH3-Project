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
	int32 EnemyTotalKillCount; //��ü �� ų ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore; //���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 OneWaveScore; //ù��° ���̺� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TwoWaveScore; //�ι�° ���̺� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 ThreeWaveScore; //����° ���̺� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 BossWaveScore; //���� ���̺� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;

	//���� �߰� �Լ�
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

	//��ü �� ų ��
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToEnemyKill(int amount);

	//������ ��ų �� �������� �Լ�
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
