#include "UI/GJ_BossHUDUserWidget.h"
#include "Components/ProgressBar.h"


void UGJ_BossHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HealthBar)
	{
		HealthBar->SetPercent(1.0f);
	}
}



void UGJ_BossHUDUserWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar && MaxHealth > 0)
	{
		float HealthPercent = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(HealthPercent);
	}
}

void UGJ_BossHUDUserWidget::AngryUI()
{
	if (AlramAnim)
	{
		PlayAnimation(AlramAnim);
	}
}
