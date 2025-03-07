#include "GameManager/GJGameMode.h"
#include "Character/GJCharacter.h"
#include "Character/GJPlayerController.h"
#include "GameManager/GJGameState.h"
#include "UI/GJHUD.h"
#include "GJGameMode.h"

AGJGameMode::AGJGameMode()
{
	//�÷��̾� ����
	DefaultPawnClass = AGJCharacter::StaticClass();
	//HUD
	HUDClass = AGJHUD::StaticClass();
	//�÷��̾� ��Ʈ�ѷ�
	PlayerControllerClass = AGJPlayerController::StaticClass();
	//���� ������Ʈ
	GameStateClass = AGJGameState::StaticClass();
}

