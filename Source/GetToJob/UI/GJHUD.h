#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GJHUD.generated.h"

class UUserWidget;
class UGJ_MainWidget;
class UGJ_GameResultWidget;
class UGJ_BossHUDUserWidget;

UENUM(BlueprintType)
enum class GJHUDState : uint8
{
	StartHUD, // 게임 시작 UI (게임 Start)
	MainHUD, // 게임 플레이 기본 UI (플레이어 상태)
	PauseHUD, // 일시정지 UI
	GameOver, // 게임 오버 UI
	GameClear, // 게임 클리어 UI
	GameFail,
	BossHUD
};

UCLASS()
class GETTOJOB_API AGJHUD : public AHUD
{
	GENERATED_BODY()
public:
	//ui 표시 및 숨기기
	void DisplayHUD(GJHUDState HUDType);
	UGJ_MainWidget* GetMainHUDWidget() const;
	UFUNCTION()
	void HideHUD(GJHUDState HUDType);
	void UpdateMainHUD();
	void UpdateMainBossHUD();
	void UpdateBossHUD(float CurrentHp, float Maxhp);
	void AngryBossHUD();
	void ShowHitEffect();
	void UpdateCrosshairSize(float NewSpread);
	void ShowFireAnim();


	//ui 위젯 클래스 (blueprint에서 설정)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StartHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGJ_MainWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGJ_GameResultWidget> ResultHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGJ_BossHUDUserWidget> BossHUDClass;
	

	UPROPERTY()
	UUserWidget* StartHUDWidget = nullptr;

	UPROPERTY()
	UGJ_MainWidget* MainHUDWidget = nullptr;

	UPROPERTY()
	UGJ_GameResultWidget* ResultHUDWidget = nullptr;

	UPROPERTY()
	UGJ_BossHUDUserWidget* BossHUDWidget = nullptr;

	

private:
	FTimerHandle HitEffectTimerHandle;
};
