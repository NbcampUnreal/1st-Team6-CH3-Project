#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_MainWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;
class UProgressBar;

UCLASS()
class GETTOJOB_API UGJ_MainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateHUD();

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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillAnimation;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Time;

	int32 CurrentKillCount = 0;
};
