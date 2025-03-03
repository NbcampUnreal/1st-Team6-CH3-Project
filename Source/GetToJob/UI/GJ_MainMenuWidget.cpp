#include "UI/GJ_MainMenuWidget.h"
#include "Components/Widget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/GJMainMenuGameMode.h"


void UGJ_MainMenuWidget::NativeConstruct()
{
	GameMode = Cast<AGJMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameStart)
	{
		GameStartButton = Cast<UButton>(GameStart->GetWidgetFromName(TEXT("Button_Input")));
		if (GameStartButton)
		{
			GameStartButton->OnClicked.AddDynamic(this, &UGJ_MainMenuWidget::OnGameStartButtonClicked);
		}
	}

	if (Setting)
	{
		SettingButton = Cast<UButton>(Setting->GetWidgetFromName(TEXT("Button_Input")));
		if (SettingButton)
		{
			SettingButton->OnClicked.AddDynamic(this, &UGJ_MainMenuWidget::OnSettingButtonClicked);
		}
	}

	if (GameOver)
	{
		GameOverButton = Cast<UButton>(GameOver->GetWidgetFromName(TEXT("Button_Input")));
		if (GameOverButton)
		{
			GameOverButton->OnClicked.AddDynamic(this, &UGJ_MainMenuWidget::OnGameOverButtonClicked);
		}
	}
}

void UGJ_MainMenuWidget::OnGameStartButtonClicked()
{
	GameMode->StartCinematic();
}

void UGJ_MainMenuWidget::OnSettingButtonClicked()
{
	if (!bSetAnim)
	{
		bSetAnim = true;
		PlayAnimation(SettingAnim);
	}
	else
	{
		bSetAnim = false;
		PlayAnimationReverse(SettingAnim);
	}
}

void UGJ_MainMenuWidget::OnGameOverButtonClicked()
{
	GameMode->QuitGame();
}
