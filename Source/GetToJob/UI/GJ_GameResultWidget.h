#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_GameResultWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;
class UGJGameInstance;
class UUserWidget;
class UButton;
class AGJPlayerController;

UCLASS()
class GETTOJOB_API UGJ_GameResultWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();
	
public:
	void WaveGameOver();
	void ClearFailUI();
	void GameClearUI();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OneWaveScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TwoWaveScore;

	UPROPERTY(meta = (BIndWidget))
	UTextBlock* ThreeWaveScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalScore;

	UPROPERTY(meta = (BindWidget))
	UImage* Seal;

	UPROPERTY(meta = (Bindwidget))
	UUserWidget* ReStart;

	UButton* RestartButton;

	UFUNCTION()
	void OnRestartButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UUserWidget* End;

	UButton* GameOverEnd;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* GameClearButton;

	UButton* ClearButton;

	UFUNCTION()
	void OnGameQuitButtonClicked();


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ScoreAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TotalScoreAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SealAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* GameOverBtAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* GameClearBtAnim;

	float DelayTime;

private:
	UGJGameInstance* GJGameInstance;
	AGJPlayerController* GJPlayerController;
	FTimerHandle ScoreUpdateTimer;

	void PlayScoreAnim();
	void PlayTotalScoreAnim();
	void CountTotalScore();
	void PlaySealAnim();
	void PlayGameOverBt();
	void PlayClearBt();

	int32 CurrentScore;
	int32 TargetScore;

	bool isClear = false;
};
