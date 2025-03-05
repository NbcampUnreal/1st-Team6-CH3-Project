#include "UI/GJHUD.h"
#include "GameManager/GJGameState.h"
#include "Blueprint/UserWidget.h"
#include "UI/GJ_MainWidget.h"
#include "UI/GJ_BossHUDUserWidget.h"
#include "UI/GJ_GameResultWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

void AGJHUD::DisplayHUD(GJHUDState HUDType)
{
	switch (HUDType)
	{
	case GJHUDState::StartHUD:
		break;
	case GJHUDState::MainHUD:
		MainHUDWidget = CreateWidget<UGJ_MainWidget>(GetWorld(), MainHUDClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
		break;
	case GJHUDState::PauseHUD:
		break;
	case GJHUDState::GameOver:
		ResultHUDWidget = CreateWidget<UGJ_GameResultWidget>(GetWorld(), ResultHUDClass);
		if (ResultHUDWidget)
		{
			ResultHUDWidget->AddToViewport();
			ResultHUDWidget->WaveGameOver();
		}
		break;
	case GJHUDState::GameClear:
		ResultHUDWidget = CreateWidget<UGJ_GameResultWidget>(GetWorld(), ResultHUDClass);
		if (ResultHUDWidget)
		{
			ResultHUDWidget->AddToViewport();
			ResultHUDWidget->GameClearUI();
		}
		break;
	case GJHUDState::GameFail:
		ResultHUDWidget = CreateWidget<UGJ_GameResultWidget>(GetWorld(), ResultHUDClass);
		if (ResultHUDWidget)
		{
			ResultHUDWidget->AddToViewport();
			ResultHUDWidget->ClearFailUI();
		}
		break;
	case GJHUDState::BossHUD:
		BossHUDWidget = CreateWidget<UGJ_BossHUDUserWidget>(GetWorld(), BossHUDClass);
		if (BossHUDWidget)
		{
			BossHUDWidget->AddToViewport();
		}
		break;
	default:
		break;
	}
}

UGJ_MainWidget* AGJHUD::GetMainHUDWidget() const
{
	return MainHUDWidget;
}

void AGJHUD::HideHUD(GJHUDState HUDType)
{
	switch (HUDType)
	{
	case GJHUDState::StartHUD:
		break;
	case GJHUDState::MainHUD:
		if (MainHUDWidget)
		{
			MainHUDWidget->RemoveFromParent();
			MainHUDWidget = nullptr;
		}
		break;
	case GJHUDState::PauseHUD:
		break;
	case GJHUDState::GameOver:
		if (ResultHUDWidget)
		{
			ResultHUDWidget->RemoveFromParent();
			ResultHUDWidget = nullptr;
		}
		break;
	case GJHUDState::GameClear:
		if (ResultHUDWidget)
		{
			ResultHUDWidget->RemoveFromParent();
			ResultHUDWidget = nullptr;
		}
		break;
	case GJHUDState::BossHUD:
		if (BossHUDWidget)
		{
			BossHUDWidget->RemoveFromParent();
			BossHUDWidget = nullptr;
		}
		break;
	default:
		break;
	}
}

void AGJHUD::UpdateMainHUD()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateHUD();;
	}
}

void AGJHUD::UpdateMainBossHUD()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateBossHUD();
	}
}

void AGJHUD::UpdateBossHUD(float CurrentHp, float Maxhp)
{
	if (BossHUDWidget)
	{
		BossHUDWidget->UpdateHealthBar(CurrentHp, Maxhp);
	}
}

void AGJHUD::AngryBossHUD()
{
	if (BossHUDWidget)
	{
		BossHUDWidget->AngryUI();
	}
}

void AGJHUD::ShowHitEffect()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ShowHitAnim();
	}
}

void AGJHUD::UpdateCrosshairSize(float NewSpread)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->SetCrosshairSize(NewSpread);
	}
}

void AGJHUD::ShowFireAnim()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->PlayFireAnim();
	}
}
