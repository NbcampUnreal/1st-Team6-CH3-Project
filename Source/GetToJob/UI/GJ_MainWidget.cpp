#include "UI/GJ_MainWidget.h"
#include "GameManager/GJGameState.h"
#include "Character/GJCharacter.h"
#include "Weapon/GJBaseGun.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/WorldSettings.h"
#include "TimerManager.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"

void UGJ_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (KillCountText && KillText)
	{
		KillCountText->SetVisibility(ESlateVisibility::Hidden);
		KillText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (HitImage)
	{
		HitImage->SetVisibility(ESlateVisibility::Hidden);
	}

	SetCrosshairSize(5.0f);
}

void UGJ_MainWidget::UpdateHUD()
{
	AGJGameState* GJGameState = GetWorld() ? Cast<AGJGameState>(GetWorld()->GetGameState()) : nullptr;
	AGJCharacter* GJCharacter = GetOwningPlayerPawn<AGJCharacter>();

	if (!GJGameState || !GJCharacter) return;

	//점수 관련
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(GJGameState->GetScore()));
	}

	//킬 카운트 올리기
	if (KillCountText)
	{
		int32 KillCount = GJGameState->GetEnemyKillCount();
		if (KillCount != 0)
		{
			KillCountText->SetVisibility(ESlateVisibility::Visible);
			KillText->SetVisibility(ESlateVisibility::Visible);

			KillCountText->SetText(FText::AsNumber(KillCount));

			if (KillCount > CurrentKillCount)
			{
				if (KillAnimation)
				{
					PlayAnimation(KillAnimation);
				}
			}

			CurrentKillCount = KillCount;
		}
	}

	//총알 관련 UI
	if (BulletText)
	{
		if (GJCharacter->CurrentGun)
		{
			BulletText->SetVisibility(ESlateVisibility::Visible);
			BulletText->SetText(FText::AsNumber(GJCharacter->CurrentGun->GetCurrentAmmo()));
		}
		else
		{
			BulletText->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (MaxBulletText)
	{
		if (GJCharacter->CurrentGun)
		{
			MaxBulletText->SetVisibility(ESlateVisibility::Visible);
			MaxBulletText->SetText(FText::AsNumber(GJCharacter->CurrentGun->GetMaxAmmo()));
		}
		else
		{
			MaxBulletText->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (GunImage)
	{
		if (GJCharacter->CurrentGun)
		{
			GunImage->SetVisibility(ESlateVisibility::Visible);

			AGJBaseGun* CurrentGun = GJCharacter->CurrentGun;

			switch (CurrentGun->GunType)
			{
			case EGunType::MiniGun:
				GunImage->SetBrushFromTexture(MiniGunImage);
				break;
			case EGunType::Revolver:
				GunImage->SetBrushFromTexture(RevolverImage);
				break;
			case EGunType::Rifle:
				GunImage->SetBrushFromTexture(RifleImage);
				break;
			case EGunType::RocketLauncher:
				GunImage->SetBrushFromTexture(RocketLauncherImage);
				break;
			default:
				break;
			}

		}
		else
		{
			GunImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//mission
	if (Mission)
	{
		int32 CurrentWave = GJGameState->CurrentWaveIndex;
		switch (CurrentWave)
		{
		case 0:
			Mission->SetText(FText::FromString(FString::Printf(TEXT("미션 1. 모든 지원자를 이기기"))));
			break;
		case 1:
			Mission->SetText(FText::FromString(FString::Printf(TEXT("미션 2. 모든 지원자를 이기기"))));
			break;
		case 2:
			Mission->SetText(FText::FromString(FString::Printf(TEXT("미션 3. 모든 지원자를 이기기"))));
			break;
		default:
			Mission->SetText(FText::FromString(FString::Printf(TEXT("미션 4. 모든 지원자를 이기기"))));
			break;
		}
	}

	//Timer
	if (Time)
	{
		float RamainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(GJGameState->LevelTimerHandle);
		Time->SetText(FText::FromString(FString::Printf(TEXT("남은 시간 : %.1f"), RamainingTime)));

		if (10.f < RamainingTime && RamainingTime <= 20.0f)
		{
			Time->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
		}
		else if (RamainingTime <= 10.0f)
		{
			Time->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
	}

	//hp
	if (HpText)
	{
		HpText->SetText(FText::AsNumber(GJCharacter->GetHealth()));
	}

	if (MaxHpText)
	{
		MaxHpText->SetText(FText::AsNumber(GJCharacter->GetMaxHealth()));
	}

	float NewHppercent = GJCharacter->GetHealth() / GJCharacter->GetMaxHealth();

	if (HealthBar)
	{
		CurrentHpPercent = FMath::Lerp(CurrentHpPercent, NewHppercent, 0.2f);
		HealthBar->SetPercent(CurrentHpPercent);
	}

	if (HealthBGBar)
	{
		BGHpPercent = FMath::Lerp(BGHpPercent, NewHppercent, 0.2f);
		HealthBGBar->SetPercent(BGHpPercent);
	}

}

void UGJ_MainWidget::ShowHitAnim()
{
	if (HitImage && HitAnim)
	{
		HitImage->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(HitAnim);
	}
}

void UGJ_MainWidget::SetCrosshairSize(float NewSpread)
{
	if (!CrosshairTop || !CrosshairBottom || !CrosshairLeft || !CrosshairRight)
		return;

	CrossSpread = NewSpread;

	FVector2D HorizontalSize(CrossLength, CrossThickness); //가로 크기
	FVector2D VerticalSize(CrossThickness, CrossLength); //세로 크기

	float x, y;
	if (UCanvasPanelSlot* BottomSlot = Cast<UCanvasPanelSlot>(CrosshairBottom->Slot))
	{
		BottomSlot->SetSize(VerticalSize);
		x = -(CrossThickness / 2);
		y = (CrossLength - 8) + CrossSpread;
		BottomSlot->SetPosition(FVector2D(x, y));
	}

	if (UCanvasPanelSlot* TopSlot = Cast<UCanvasPanelSlot>(CrosshairTop->Slot))
	{
		TopSlot->SetSize(VerticalSize);
		x = -(CrossThickness / 2);
		y = -CrossLength - CrossSpread;
		TopSlot->SetPosition(FVector2D(x, y));
	}

	if (UCanvasPanelSlot* LeftSlot = Cast<UCanvasPanelSlot>(CrosshairLeft->Slot))
	{
		LeftSlot->SetSize(HorizontalSize);
		x = -CrossLength - CrossSpread;
		y = -(CrossThickness / 2);
		LeftSlot->SetPosition(FVector2D(x, y));
	}

	if (UCanvasPanelSlot* RightSlot = Cast<UCanvasPanelSlot>(CrosshairRight->Slot))
	{
		RightSlot->SetSize(HorizontalSize);
		x = (CrossLength + CrossSpread) - CrossLength;
		y = -(CrossThickness / 2);
		RightSlot->SetPosition(FVector2D(x, y));
	}
}




