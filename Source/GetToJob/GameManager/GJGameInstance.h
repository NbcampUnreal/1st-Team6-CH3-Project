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


};
