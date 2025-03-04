#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_MainMenuWidget.generated.h"

class UUserWidget;
class UButton;
class UWidgetAnimation;
class AGJMainMenuGameMode;

UCLASS()
class GETTOJOB_API UGJ_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	

private:
	UPROPERTY(meta = (BindWidget))
	UUserWidget* GameStart;

	UButton* GameStartButton;

	UFUNCTION()
	void OnGameStartButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UUserWidget* Setting;

	UButton* SettingButton;

	UFUNCTION()
	void OnSettingButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UUserWidget* GameOver;

	UButton* GameOverButton;

	UFUNCTION()
	void OnGameOverButtonClicked();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SettingAnim;

	bool bSetAnim;

	AGJMainMenuGameMode* GameMode;
};
