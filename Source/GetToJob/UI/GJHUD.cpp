#include "UI/GJHUD.h"
#include "Character/GJPlayerController.h"
#include "Character//GJCharacter.h"
#include "Weapon/GJBaseGun.h"
#include "GameManager/GJGameState.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void AGJHUD::DisplayHUD(GJHUDState HUDType)
{
	switch (HUDType)
	{
	case GJHUDState::StartHUD:
		break;
	case GJHUDState::MainHUD:
		MainHUDWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
		break;
	case GJHUDState::HitEffect:
		break;
	case GJHUDState::PauseHUD:
		break;
	case GJHUDState::GameOver:
		break;
	case GJHUDState::GameClear:
		break;
	default:
		break;
	}
}

void AGJHUD::HideHUD(GJHUDState HUDType)
{
	switch (HUDType)
	{
	case GJHUDState::StartHUD:
		break;
	case GJHUDState::MainHUD:
		break;
	case GJHUDState::HitEffect:
		break;
	case GJHUDState::PauseHUD:
		break;
	case GJHUDState::CrossHairHUD:
		break;
	case GJHUDState::GameOver:
		break;
	case GJHUDState::GameClear:
		break;
	default:
		break;
	}
}

void AGJHUD::UpdateMainHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			if (MainHUDWidget)
			{
				AGJGameState* GJGameState = Cast<AGJGameState>(GetWorld()->GetGameState());
				AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerController->GetPawn());
				if (UTextBlock* TimeText = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RamainingTime = GetWorldTimerManager().GetTimerRemaining(GJGameState->LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("남은 시간 : %.1f"), RamainingTime ? RamainingTime : 0)));

					if (10.0f < RamainingTime && RamainingTime <= 20.0f)
					{
						TimeText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
					}
					else if (RamainingTime <= 10.0f)
					{
						TimeText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
					}
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GJGameState->GetScore())));
				}
				
				if (UTextBlock* HpText = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("TextHp"))))
				{
					HpText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GJCharacter->GetHealth())));
				}

				if (UTextBlock* MaxHpText = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("TextMaxHp"))))
				{
					//MaxHpText->SetText(FText::FromString(FString::Printf(TEXT("%d"), )));
				}

				if (UTextBlock* MissionText = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("Mission"))))
				{
					switch (GJGameState->CurrentWaveIndex)
					{
					case 0:
						MissionText->SetText(FText::FromString(FString::Printf(TEXT("미션 1. 모든 지원자를 이기기"))));
						break;
					case 1:
						MissionText->SetText(FText::FromString(FString::Printf(TEXT("미션 2. 모든 지원자를 이기기"))));
						break;
					case 2:
						MissionText->SetText(FText::FromString(FString::Printf(TEXT("미션 3. 모든 지원자를 이기기"))));
						break;
					default:
						MissionText->SetText(FText::FromString(FString::Printf(TEXT("미션 4. 보스를 이기기"))));
						break;
					}
				}

				if (UTextBlock* CurrentBullte = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("BulletText"))))
				{
					//if (GJCharacter && GJCharacter->CurrentGun)
					//{
					//	CurrentBullte->SetVisibility(ESlateVisibility::Visible);
					//	CurrentBullte->SetText(FText::FromString(FString::Printf(TEXT("%d"), GJCharacter->CurrentGun ? GJCharacter->CurrentGun->GetCurrentAmmo() : 0)));
					//}
					//else
					//{
					//	CurrentBullte->SetVisibility(ESlateVisibility::Hidden);
					//	UE_LOG(LogTemp, Warning, TEXT("총없서오"));
					//}
				}

				if (UTextBlock* KillCount = Cast<UTextBlock>(MainHUDWidget->GetWidgetFromName(TEXT("KillCountText"))))
				{
					KillCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), GJGameState->GetEnemyKillCount())));
				}

			}
		}
	}
}

void AGJHUD::ShowHitEffect()
{
}
