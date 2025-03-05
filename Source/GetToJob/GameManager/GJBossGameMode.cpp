#include "GameManager/GJBossGameMode.h"
#include "GameManager/GJGameInstance.h"
#include "GameManager/GJBossGameState.h"
#include "Character/GJPlayerController.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

AGJBossGameMode::AGJBossGameMode()
{
}

void AGJBossGameMode::BeginPlay()
{

}

void AGJBossGameMode::OnBossDefeated()
{
	UGJGameInstance* GameInstance = Cast<UGJGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	int32 FinalScore = GameInstance->TotalScore;

	if (FinalScore > ClearScoreCheck)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
			{
				GJPlayerController->GameClear();
			}
		}
	}
	else
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
			{
				GJPlayerController->GameClearFail();
			}
		}
	}
}

void AGJBossGameMode::StartCinematic()
{
	ALevelSequenceActor* SequenceActor = nullptr;
	ULevelSequencePlayer* SequencePlayer = nullptr;

	ULevelSequence* LevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/BossScene/Ending.Ending"));

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = false;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, SequenceActor);

	SequencePlayer->OnFinished.AddDynamic(this, &AGJBossGameMode::OnCinematicFinished);

	SequencePlayer->Play();
}

void AGJBossGameMode::OnCinematicFinished()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AGJPlayerController* GJPlayerController = Cast<AGJPlayerController>(PlayerController))
		{
			GJPlayerController->QuitGame();
		}
	}
}
