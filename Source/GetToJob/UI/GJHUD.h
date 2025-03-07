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
	StartHUD, // ���� ���� UI (���� Start)
	MainHUD, // ���� �÷��� �⺻ UI (�÷��̾� ����)
	PauseHUD, // �Ͻ����� UI
	GameOver, // ���� ���� UI
	GameClear, // ���� Ŭ���� UI
	GameFail,
	BossHUD
};

UCLASS()
class GETTOJOB_API AGJHUD : public AHUD
{
	GENERATED_BODY()
public:
	//ui ǥ�� �� �����
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


	//ui ���� Ŭ���� (blueprint���� ����)
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
