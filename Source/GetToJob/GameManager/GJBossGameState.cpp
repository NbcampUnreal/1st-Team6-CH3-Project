#include "GameManager/GJBossGameState.h"
#include "Character/GJPlayerController.h"
#include "GameManager/GJGameInstance.h"
#include "GameManager/GJBossGameMode.h"
#include "UI/GJHUD.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

AGJBossGameState::AGJBossGameState()
{
	Score = 0;
	EnemyKillCount = 0;

}

int32 AGJBossGameState::GetScore()
{
	return Score;
}

void AGJBossGameState::AddScore(int32 Amount)
{
	Score += Amount;
}

int32 AGJBossGameState::GetEnemyKillCount()
{
	return EnemyKillCount;
}

void AGJBossGameState::AddEnemyKillCount(int32 Amount)
{
	EnemyKillCount += Amount;
}

void AGJBossGameState::PlaySound()
{
	if (BossBGM)
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(this, BossBGM);
	}
}

void AGJBossGameState::StopSound()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
}

void AGJBossGameState::BeginPlay()
{
	Super::BeginPlay();

	BossStart();
	PlaySound();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				GetWorldTimerManager().SetTimer(
					UIUpdateTimerHandle,
					HUD,
					&AGJHUD::UpdateMainBossHUD,
					0.1f,
					true
				);
			}
		}
	}
}

void AGJBossGameState::BossStart()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				HUD->DisplayHUD(GJHUDState::MainHUD);
				HUD->DisplayHUD(GJHUDState::BossHUD);
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGJBossGameState::BossFail,
		BossLimitTime,
		false
	);
}

void AGJBossGameState::BossFail()
{
	//타이머 초과하면, 타이머 초기화
	//결과창에 실패 출력.

	GetWorldTimerManager().ClearAllTimersForObject(this);

	StopSound();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			GJPlayerController->GameOver();

			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				HUD->HideHUD(GJHUDState::BossHUD);
			}
		}
	}
}

void AGJBossGameState::SetBossDefeated()
{
	if (HasAuthority())
	{
		bIsBossDefeated = true;

		UGJGameInstance* GameInstance = Cast<UGJGameInstance>(GetGameInstance());
		if (!GameInstance) return;

		StopSound();

		GetWorldTimerManager().ClearAllTimersForObject(this);
		//보스 HUD 지우기
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
			{
				if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
				{
					HUD->HideHUD(GJHUDState::BossHUD);
				}
			}
		}

		AGJBossGameMode* GameMode = Cast<AGJBossGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnBossDefeated();
		}

	}
}
