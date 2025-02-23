#include "UI/GJHUD.h"
#include "Character/GJPlayerController.h"
#include "Character//GJCharacter.h"
#include "Weapon/GJBaseGun.h"
#include "GameManager/GJGameState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "UI/GJ_MainWidget.h"
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
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateHUD();;
	}
}

void AGJHUD::ShowHitEffect()
{

}
