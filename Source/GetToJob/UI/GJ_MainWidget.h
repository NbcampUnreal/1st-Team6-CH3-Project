#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_MainWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;
class UProgressBar;
class UBorder;
class AGJBaseGun;
class AGjMiniGun;

UCLASS()
class GETTOJOB_API UGJ_MainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateHUD();
	void UpdateBossHUD();
	void ShowHitAnim();
	UFUNCTION(BlueprintCallable, Category = "Crosshair Test")
	void SetCrosshairSize(float NewSpread);

	void PlayFireAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrossSpread = 0.0f;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HpText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHpText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBGBar;

	float CurrentHpPercent = 1.0f;
	float BGHpPercent = 1.0f;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mission;

	UPROPERTY(meta = (BindWidget))
	UImage* GunImage;

	UPROPERTY(EditAnywhere, Category = "Gun Images")
	UTexture2D* RevolverImage;

	UPROPERTY(EditAnywhere, Category = "Gun Images")
	UTexture2D* MiniGunImage;

	UPROPERTY(EditAnywhere, Category = "Gun Images")
	UTexture2D* RifleImage;

	UPROPERTY(EditAnywhere, Category = "Gun Images")
	UTexture2D* RocketLauncherImage;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillAnimation;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Time;

	int32 CurrentKillCount = 0;

	UPROPERTY(meta = (BindWidget))
	UImage* HitImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HitAnim;

	//crosshair
	UPROPERTY(meta = (BindWidget))
	UBorder* CrosshairTop;

	UPROPERTY(meta = (BindWidget))
	UBorder* CrosshairBottom;

	UPROPERTY(meta = (BindWidget))
	UBorder* CrosshairLeft;

	UPROPERTY(meta = (BindWidget))
	UBorder* CrosshairRight;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FireAnim;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrossLength = 10.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrossThickness = 5.0f;

public:
	//Ult
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Ult;

	float CurrentUltimateGauge;
	float MaxUltimateGauge;


	UFUNCTION()
	void UpdateUltimateGauge();
};
