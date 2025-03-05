#include "GameManager/GJGameState.h"
#include "GameManager/GJGameInstance.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GJHUD.h"
#include "Character/GJPlayerController.h"

AGJGameState::AGJGameState()
{
	Score = 0;
	EnemyKillCount = 0;
	//LevelLimitTime = 60.0f;
	CurrentWaveIndex = 0;
	//MaxLevels = 3;
	//LevelMapNames = { "LoopStage", "LoopStage", "LoopStage" };
}

int32 AGJGameState::GetScore() const
{
	return Score;
}

void AGJGameState::AddScore(int32 Amount)
{
	Score += Amount;
}

int32 AGJGameState::GetEnemyKillCount() const
{
	return EnemyKillCount;
}

void AGJGameState::AddEnemyKillCount(int32 Amount)
{
	EnemyKillCount += Amount;
}

void AGJGameState::BeginPlay()
{
	Super::BeginPlay();

	StartWave();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				GetWorldTimerManager().SetTimer(
					UIUpdateTimerHandle,
					HUD,
					&AGJHUD::UpdateMainHUD,
					0.1f,
					true
				);
			}
		}
	}
}

void AGJGameState::StartWave()
{
	//CurrentLevel Update
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGJGameInstance* GJGameInstance = Cast<UGJGameInstance>(GameInstance);
		if (GJGameInstance)
		{
			CurrentWaveIndex = GJGameInstance->CurrentWaveIndex;
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				HUD->DisplayHUD(GJHUDState::MainHUD);
			}
		}
	}

	//Level Timer
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGJGameState::NextWave,
		LevelLimitTime,
		false
	);
}

void AGJGameState::NextWave()
{
	
	//NextWave 
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGJGameInstance* GJGameInstance = Cast<UGJGameInstance>(GameInstance);
		if (GJGameInstance)
		{
			//TotalScore, EnemyKillCount Save
			GJGameInstance->AddToScore(Score);
			GJGameInstance->AddToEnemyKill(EnemyKillCount);
			// 캐릭터 상태 저장
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (AGJCharacter* Character = Cast<AGJCharacter>(PlayerController->GetPawn()))
				{
					GJGameInstance->SaveCharacterState(Character);

					// 유예시간 동안 무적 설정
					Character->bIsInvincible = true;
					// 플레이어 입력 차단 (유예시간 동안 움직이지 못하게)
					Character->DisableInput(PlayerController);
					UE_LOG(LogTemp, Warning, TEXT("Player input disabled for wave transition."));
				}
			}

			//Waves Score Save
			switch (CurrentWaveIndex)
			{
			case 0:
				GJGameInstance->SaveOneWaveScore(Score);
				break;
			case 1:
				GJGameInstance->SaveTwoWaveScore(Score);
				break;
			case 2:
				GJGameInstance->SaveThreeWaveScore(Score);
				break;
			case 3:
				GJGameInstance->SaveBossScore(Score);
				break;
			}

			//Wave Plus, Save
			CurrentWaveIndex++;
			GJGameInstance->CurrentWaveIndex = CurrentWaveIndex;
		}
	}

	if (LevelMapNames.IsValidIndex(CurrentWaveIndex))
	{
		// 유예시간 (5초) 후에 `LoadNextLevel` 함수 실행
		float DelayBeforeNextWave = 5.0f;
		GetWorldTimerManager().SetTimer(
			WaveTransitionTimerHandle,  // 타이머 핸들
			this,
			&AGJGameState::LoadNextLevel,  // 호출할 함수
			DelayBeforeNextWave,
			false
		);
	}
	

	UE_LOG(LogTemp, Warning, TEXT("NextLevel %f, %d"), LevelLimitTime, CurrentWaveIndex);
}

void AGJGameState::LoadNextLevel()
{
	// 입력 다시 활성화
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJCharacter* Character = Cast<AGJCharacter>(PlayerController->GetPawn()))
		{
			// 무적 해제
			Character->bIsInvincible = false;
			// 입력 정상화
			Character->EnableInput(PlayerController);
			UE_LOG(LogTemp, Warning, TEXT("Player input re-enabled after wave transition."));
		}
	}

	// UI 및 타이머 정리
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(UIUpdateTimerHandle);

	// 새로운 레벨 열기
	UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentWaveIndex]);
}



