#include "Weapon/GJScope.h"
#include "Weapon/GJBaseGun.h"
#include "Character/GJCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AGJScope::AGJScope()
{
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);


	ZoomFOV = 30.0f; // 조준 시 FOV 값

	CameraRelativeX = 40.0f; // 조준 시 카메라 X위치
	CameraRelativeY = 0.0f; // 조준 시 카메라 Y위치
	CameraRelativeZ = 70.0f; // 조준 시 카메라 Z위치
}

// 줌인
void AGJScope::EnableScopeView()
{
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->SetFOV(ZoomFOV);
	}

	// 1인칭으로 전환 (CameraBoom 제거 및 FollowCamera 위치 조정)
	AGJCharacter* PlayerCharacter = Cast<AGJCharacter>(PlayerController->GetPawn());
	if (PlayerCharacter && PlayerCharacter->SpringArmComp && PlayerCharacter->CameraComp)
	{
		// 카메라 붐을 비활성화하여 1인칭 시점으로 변경
		PlayerCharacter->SpringArmComp->bUsePawnControlRotation = false;
		PlayerCharacter->SpringArmComp->TargetArmLength = 0.0f; // 카메라를 캐릭터에 붙임
		PlayerCharacter->CameraComp->SetRelativeLocation(FVector(CameraRelativeX, CameraRelativeY, CameraRelativeZ)); // 머리 위치로 조정
		PlayerCharacter->CameraComp->bUsePawnControlRotation = true; // 카메라 회전 허용
	}
}

// 줌아웃
void AGJScope::DisableScopeView()
{
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->SetFOV(DefaultFOV);
	}

	// 3인칭으로 복귀 (CameraBoom 활성화 및 FollowCamera 원래 위치 복원)
	AGJCharacter* PlayerCharacter = Cast<AGJCharacter>(PlayerController->GetPawn());
	if (PlayerCharacter && PlayerCharacter->SpringArmComp && PlayerCharacter->CameraComp)
	{
		// 카메라 붐을 다시 활성화하여 3인칭 복귀
		PlayerCharacter->SpringArmComp->bUsePawnControlRotation = true;
		PlayerCharacter->SpringArmComp->TargetArmLength = 300.0f; // 기본적인 3인칭 거리로 설정
		PlayerCharacter->CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 원래 위치로 복구
		PlayerCharacter->CameraComp->bUsePawnControlRotation = false; // 3인칭 시 회전 비활성화
	}
}

void AGJScope::AttachToGun(AGJBaseGun* Gun)
{
	Super::AttachToGun(Gun);

	if (Gun && Gun->GetOwner())
	{
		// 카메라는 캐릭터의 것인데 왜 PlayerController에 PlayerCameraManager가 있고 거기서 FOVAngle을 가져올 수 있는거지??
		PlayerController = Cast<APlayerController>(Gun->GetOwner()->GetInstigatorController());
		if (PlayerController)
		{
			DefaultFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
		}
	}
}

void AGJScope::DetachFromGun()
{
	Super::DetachFromGun();

	DisableScopeView();
	Super::DetachFromGun();
}
