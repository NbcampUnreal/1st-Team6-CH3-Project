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


};
