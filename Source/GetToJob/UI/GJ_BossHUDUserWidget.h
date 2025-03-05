#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_BossHUDUserWidget.generated.h"

class UProgressBar;
class UWidgetAnimation;

UCLASS()
class GETTOJOB_API UGJ_BossHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AlramAnim;

public:
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	void AngryUI();

};
