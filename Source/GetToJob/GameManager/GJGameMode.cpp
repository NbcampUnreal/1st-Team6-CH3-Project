#include "GameManager/GJGameMode.h"
#include "Character/GJCharacter.h"
#include "Character/GJPlayerController.h"
#include "GJGameMode.h"

AGJGameMode::AGJGameMode()
{
	//�÷��̾� ����
	DefaultPawnClass = AGJCharacter::StaticClass();
	//�÷��̾� ��Ʈ�ѷ�
	PlayerControllerClass = AGJPlayerController::StaticClass();
	//���� ������Ʈ
}

