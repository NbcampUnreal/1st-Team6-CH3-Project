#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GJHUD.generated.h"

class UUserWidget;
class UGJ_MainWidget;

UENUM(BlueprintType)
enum class GJHUDState : uint8
{
	StartHUD, // ���� ���� UI (���� Start)
	MainHUD, // ���� �÷��� �⺻ UI (�÷��̾� ����)
	HitEffect, // �ǰ� ȿ�� (��� ǥ��)
	PauseHUD, // �Ͻ����� UI
	CrossHairHUD, // ũ�ν���� UI
	GameOver, // ���� ���� UI
	GameClear // ���� Ŭ���� UI
};

UCLASS()
class GETTOJOB_API AGJHUD : public AHUD
{
	GENERATED_BODY()
public:
	//ui ǥ�� �� �����
	void DisplayHUD(GJHUDState HUDType);
	UFUNCTION()
	void HideHUD(GJHUDState HUDType);
	void UpdateMainHUD();
	void ShowHitEffect();

	//ui ���� Ŭ���� (blueprint���� ����)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StartHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGJ_MainWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HitEffectClass;

	UPROPERTY()
	UUserWidget* StartHUDWidget = nullptr;

	UPROPERTY()
	UGJ_MainWidget* MainHUDWidget = nullptr;

	UPROPERTY()
	UUserWidget* HitEffectWidget = nullptr;

	

private:
	FTimerHandle HitEffectTimerHandle;
};
