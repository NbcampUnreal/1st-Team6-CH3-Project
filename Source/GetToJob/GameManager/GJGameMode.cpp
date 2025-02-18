#include "GameManager/GJGameMode.h"
#include "Character/GJCharacter.h"
#include "Character/GJPlayerController.h"
#include "GJGameMode.h"

AGJGameMode::AGJGameMode()
{
	//플레이어 지정
	DefaultPawnClass = AGJCharacter::StaticClass();
	//플레이어 컨트롤러
	PlayerControllerClass = AGJPlayerController::StaticClass();
	//게임 스테이트
}

