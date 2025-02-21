#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GJHUD.generated.h"

class UUserWidget;

UENUM(BlueprintType)
enum class GJHUDState : uint8
{
	StartHUD, // 게임 시작 UI (게임 Start)
	MainHUD, // 게임 플레이 기본 UI (플레이어 상태)
	HitEffect, // 피격 효과 (잠깐 표시)
	PauseHUD, // 일시정지 UI
	CrossHairHUD, // 크로스헤어 UI
	GameOver, // 게임 오버 UI
	GameClear // 게임 클리어 UI
};

UCLASS()
class GETTOJOB_API AGJHUD : public AHUD
{
	GENERATED_BODY()
public:
	//ui 표시 및 숨기기
	void DisplayHUD(GJHUDState HUDType);
	UFUNCTION()
	void HideHUD(GJHUDState HUDType);
	void UpdateMainHUD();
	void ShowHitEffect();

	//ui 위젯 클래스 (blueprint에서 설정)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StartHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HitEffectClass;

	UPROPERTY()
	UUserWidget* StartHUDWidget = nullptr;

	UPROPERTY()
	UUserWidget* MainHUDWidget = nullptr;

	UPROPERTY()
	UUserWidget* HitEffectWidget = nullptr;

	

private:
	FTimerHandle HitEffectTimerHandle;
};
