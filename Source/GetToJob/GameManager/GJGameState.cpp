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
			// ĳ���� ���� ����
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (AGJCharacter* Character = Cast<AGJCharacter>(PlayerController->GetPawn()))
				{
					GJGameInstance->SaveCharacterState(Character);

					// �����ð� ���� ���� ����
					Character->bIsInvincible = true;
					// �÷��̾� �Է� ���� (�����ð� ���� �������� ���ϰ�)
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
		// �����ð� (5��) �Ŀ� `LoadNextLevel` �Լ� ����
		float DelayBeforeNextWave = 5.0f;
		GetWorldTimerManager().SetTimer(
			WaveTransitionTimerHandle,  // Ÿ�̸� �ڵ�
			this,
			&AGJGameState::LoadNextLevel,  // ȣ���� �Լ�
			DelayBeforeNextWave,
			false
		);
	}
	

	UE_LOG(LogTemp, Warning, TEXT("NextLevel %f, %d"), LevelLimitTime, CurrentWaveIndex);
}

void AGJGameState::LoadNextLevel()
{
	// �Է� �ٽ� Ȱ��ȭ
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJCharacter* Character = Cast<AGJCharacter>(PlayerController->GetPawn()))
		{
			// ���� ����
			Character->bIsInvincible = false;
			// �Է� ����ȭ
			Character->EnableInput(PlayerController);
			UE_LOG(LogTemp, Warning, TEXT("Player input re-enabled after wave transition."));
		}
	}

	// UI �� Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(UIUpdateTimerHandle);

	// ���ο� ���� ����
	UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentWaveIndex]);
}



