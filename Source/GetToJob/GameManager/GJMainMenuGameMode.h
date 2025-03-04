#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GJMainMenuGameMode.generated.h"

class UGJ_MainMenuWidget;
class APlayerController;
class USoundBase;
class UAudioComponent;

UCLASS()
class GETTOJOB_API AGJMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AGJMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGJ_MainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	UGJ_MainMenuWidget* MainMenuWidget;

	APlayerController* Controller;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* MainMenuBGM;

	UPROPERTY()
	UAudioComponent* AudioComponent;

	UFUNCTION(BlueprintCallable)
	void StartCinematic();

	UFUNCTION()
	void OnCinematicFinished();

	void QuitGame();

	void PlaySound();
	void StopSound();
};
