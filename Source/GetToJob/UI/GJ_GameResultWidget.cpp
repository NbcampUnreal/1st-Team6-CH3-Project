#include "UI/GJ_GameResultWidget.h"
#include "GameManager/GJGameInstance.h"
#include "Character/GJPlayerController.h"
#include "GameManager/GJBossGameMode.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "TimerManager.h"
#include "GameFramework/WorldSettings.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UGJ_GameResultWidget::NativeConstruct()
{
	if (GetWorld())
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();

		if (GameInstance)
		{
			GJGameInstance = Cast<UGJGameInstance>(GameInstance);
		}
	}

	GJPlayerController = Cast<AGJPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	CurrentScore = 0;
	TargetScore = GJGameInstance->GetTotalScore();

	if (ReStart)
	{
		RestartButton = Cast<UButton>(ReStart->GetWidgetFromName(TEXT("Bt_Input")));

		if (RestartButton)
		{
			RestartButton->OnClicked.AddDynamic(this, &UGJ_GameResultWidget::OnRestartButtonClicked);
		}
	}

	if (End)
	{
		GameOverEnd = Cast<UButton>(End->GetWidgetFromName(TEXT("Bt_Input")));

		if (GameOverEnd)
		{
			GameOverEnd->OnClicked.AddDynamic(this, &UGJ_GameResultWidget::OnGameQuitButtonClicked);
		}
	}

	if (GameClearButton)
	{
		ClearButton = Cast<UButton>(GameClearButton->GetWidgetFromName(TEXT("Bt_Input")));

		if (ClearButton)
		{
			ClearButton->OnClicked.AddDynamic(this, &UGJ_GameResultWidget::OnGameClearButtonClicked);
		}
	}
}

void UGJ_GameResultWidget::WaveGameOver()
{
	if (ResultText)
	{
		ResultText->SetText(FText::FromString(FString::Printf(TEXT("면접 탈락"))));
		ResultText->SetColorAndOpacity(FSlateColor(FLinearColor(0.3f, 0.0f, 0.0f, 0.0f)));
	}

	if (KillCount)
	{
		KillCount->SetText(FText::FromString(FString::Printf(TEXT("해치운 지원자 : %d"), GJGameInstance->GetEnemyTotalKillCount())));
	}

	if (OneWaveScore)
	{
		OneWaveScore->SetText(FText::FromString(FString::Printf(TEXT("서류 면접 : %d"), GJGameInstance->GetOneWaveScore())));
	}

	if (TwoWaveScore)
	{
		TwoWaveScore->SetText(FText::FromString(FString::Printf(TEXT("인성 면접 : %d"), GJGameInstance->GetTwoWaveScore())));
	}

	if (ThreeWaveScore)
	{
		ThreeWaveScore->SetText(FText::FromString(FString::Printf(TEXT("직무 테스트 : %d"), GJGameInstance->GetThreeWaveScore())));
	}

	if (BossScore)
	{
		BossScore->SetText(FText::FromString(FString::Printf(TEXT("임원 면접 : %d"), GJGameInstance->GetBossScore())));
	}

	if (TotalScore)
	{
		TotalScore->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : 0"))));
	}

	if (Seal)
	{
		Seal->SetBrushFromTexture(FailSeal);
	}

	isClear = false;
	PlayScoreAnim();
}

void UGJ_GameResultWidget::ClearFailUI()
{
	if (ResultText)
	{
		ResultText->SetText(FText::FromString(FString::Printf(TEXT("면접 결과"))));
		ResultText->SetColorAndOpacity(FSlateColor(FLinearColor(0.3f, 0.0f, 0.0f, 0.0f)));
	}

	if (KillCount)
	{
		KillCount->SetText(FText::FromString(FString::Printf(TEXT("해치운 지원자 : %d"), GJGameInstance->GetEnemyTotalKillCount())));
	}

	if (OneWaveScore)
	{
		OneWaveScore->SetText(FText::FromString(FString::Printf(TEXT("1차 면접 : %d"), GJGameInstance->GetOneWaveScore())));
	}

	if (TwoWaveScore)
	{
		TwoWaveScore->SetText(FText::FromString(FString::Printf(TEXT("2차 면접 : %d"), GJGameInstance->GetTwoWaveScore())));
	}

	if (ThreeWaveScore)
	{
		ThreeWaveScore->SetText(FText::FromString(FString::Printf(TEXT("3차 면접 : %d"), GJGameInstance->GetThreeWaveScore())));
	}

	if (BossScore)
	{
		BossScore->SetText(FText::FromString(FString::Printf(TEXT("최종 면접 : %d"), GJGameInstance->GetBossScore())));
	}

	if (TotalScore)
	{
		TotalScore->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : 0"))));
	}

	if (Seal)
	{
		Seal->SetBrushFromTexture(FailSeal);
	}

	isClear = false;
	PlayScoreAnim();
}

void UGJ_GameResultWidget::GameClearUI()
{
	Super::SynchronizeProperties();

	if (ResultText)
	{
		ResultText->SetText(FText::FromString(FString::Printf(TEXT("면접 결과"))));
		ResultText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.6f, 0.0f, 1.0f)));
	}

	if (KillCount)
	{
		KillCount->SetText(FText::FromString(FString::Printf(TEXT("해치운 지원자 : %d"), GJGameInstance->GetEnemyTotalKillCount())));
	}

	if (OneWaveScore)
	{
		OneWaveScore->SetText(FText::FromString(FString::Printf(TEXT("1차 면접 : %d"), GJGameInstance->GetOneWaveScore())));
	}

	if (TwoWaveScore)
	{
		TwoWaveScore->SetText(FText::FromString(FString::Printf(TEXT("2차 면접 : %d"), GJGameInstance->GetTwoWaveScore())));
	}

	if (ThreeWaveScore)
	{
		ThreeWaveScore->SetText(FText::FromString(FString::Printf(TEXT("3차 면접 : %d"), GJGameInstance->GetThreeWaveScore())));
	}

	if (BossScore)
	{
		BossScore->SetText(FText::FromString(FString::Printf(TEXT("최종 면접 : %d"), GJGameInstance->GetBossScore())));
	}

	if (TotalScore)
	{
		TotalScore->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : 0"))));
	}

	if (Seal)
	{
		Seal->SetBrushFromTexture(ClearSeal);
	}

	PlayScoreAnim();

	isClear = true;
}

void UGJ_GameResultWidget::OnRestartButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (GJGameInstance->CurrentWaveIndex < 3)
		{
			UGameplayStatics::OpenLevel(World, "MainLevel");
		}
		else
		{
			UGameplayStatics::OpenLevel(World, "BossStage");
		}
	}
}

void UGJ_GameResultWidget::OnGameClearButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, "EndingStage");
	}
}

void UGJ_GameResultWidget::OnGameQuitButtonClicked()
{
	GJPlayerController->QuitGame();
}

void UGJ_GameResultWidget::PlayScoreAnim()
{
	if (ScoreAnim)
	{
		PlayAnimation(ScoreAnim);


		GetWorld()->GetTimerManager().ClearTimer(ScoreUpdateTimer);
		GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::PlayTotalScoreAnim, 4.5f, false);
	}

}

void UGJ_GameResultWidget::PlayTotalScoreAnim()
{
	if (TotalScoreAnim)
	{
		PlayAnimation(TotalScoreAnim);

		GetWorld()->GetTimerManager().ClearTimer(ScoreUpdateTimer);
		GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::CountTotalScore, 1.0f, false);
	}
}

void UGJ_GameResultWidget::CountTotalScore()
{
	if (CurrentScore < TargetScore)
	{
		// 증가량 조절 (예: 남은 점수의 5%만큼 증가, 최소 1)
		int32 IncreaseAmount = FMath::Max(1, FMath::RoundToInt((TargetScore - CurrentScore) * 0.05f));
		CurrentScore += IncreaseAmount;

		if (TotalScore)
		{
			TotalScore->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : %d"), CurrentScore)));
		}

		// 타이머를 설정하여 계속 호출
		GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::CountTotalScore, 0.05f, false);
	}
	else
	{
		CurrentScore = TargetScore;  // 점수가 정확히 목표 점수에 도달하도록 보정
		GetWorld()->GetTimerManager().ClearTimer(ScoreUpdateTimer);

		GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::PlaySealAnim, 1.0f, false);
	}
}

void UGJ_GameResultWidget::PlaySealAnim()
{
	if (SealAnim)
	{
		Seal->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(SealAnim);

		GetWorld()->GetTimerManager().ClearTimer(ScoreUpdateTimer);

		if (!isClear)
		{
			GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::PlayGameOverBt, 1.0f, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimer, this, &UGJ_GameResultWidget::PlayClearBt, 1.0f, false);
		}
	}
}

void UGJ_GameResultWidget::PlayGameOverBt()
{
	if (GameOverBtAnim)
	{
		ReStart->SetVisibility(ESlateVisibility::Visible);
		End->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(GameOverBtAnim);
	}
}

void UGJ_GameResultWidget::PlayClearBt()
{
	if (GameClearBtAnim)
	{
		GameClearButton->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(GameClearBtAnim);
	}
}





