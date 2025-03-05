#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Weapon/GJBaseGun.h"
#include "Weapon/GJMiniGun.h"
#include "GJGameInstance.generated.h"

UCLASS()
class GETTOJOB_API UGJGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGJGameInstance();

	// 추가: 캐릭터 상태 저장을 위한 변수들
	float SavedHealth;
	float SavedMaxHealth;
	float SavedUltimateGauge;
	TArray<TSubclassOf<AGJBaseGun>> SavedWeapons;  // 소지 무기 리스트
	int32 SavedCurrentWeaponIndex; // 현재 장착 무기
	TMap<TSubclassOf<AGJBaseGun>, int32> SavedAmmoMap; // 무기별 탄약량 저장

	// 상태 저장 메서드
	void SaveCharacterState(class AGJCharacter* Character);
	void LoadCharacterState(class AGJCharacter* Character);

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
