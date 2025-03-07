#include "GameManager/GJMainMenuGameMode.h"
#include "UI/GJ_MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

AGJMainMenuGameMode::AGJMainMenuGameMode()
{
	MainMenuWidgetClass = nullptr;
}

void AGJMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UGJ_MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}

	if (Controller)
	{
		Controller->bShowMouseCursor = true;
	}

	PlaySound();
}

void AGJMainMenuGameMode::StartCinematic()
{
	MainMenuWidget->RemoveFromParent();

	if (Controller)
	{
		Controller->bShowMouseCursor = false;
	}

	StopSound();
	
	ALevelSequenceActor* SequenceActor = nullptr;
	ULevelSequencePlayer* SequencePlayer = nullptr;

	ULevelSequence* LevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Cine/GetToJobCutScene.GetToJobCutScene"));

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = false;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, SequenceActor);

	SequencePlayer->OnFinished.AddDynamic(this, &AGJMainMenuGameMode::OnCinematicFinished);

	SequencePlayer->Play();
}

void AGJMainMenuGameMode::OnCinematicFinished()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainLevel"));
}

void AGJMainMenuGameMode::QuitGame()
{
	Controller->bShowMouseCursor = false;

	UKismetSystemLibrary::QuitGame
	(
		GetWorld(),
		Controller,
		EQuitPreference::Quit,
		false
	);
}

void AGJMainMenuGameMode::PlaySound()
{
	if (MainMenuBGM)
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(this, MainMenuBGM);
	}
}

void AGJMainMenuGameMode::StopSound()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
}


