#include "Weapon/GJScope.h"
#include "Weapon/GJBaseGun.h"
#include "Character/GJCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AGJScope::AGJScope()
{
	ZoomFOV = 30.0f; // 조준 시 FOV 값

	CameraRelativeX = 40.0f; // 조준 시 카메라 X위치
	CameraRelativeY = 0.0f; // 조준 시 카메라 Y위치
	CameraRelativeZ = 70.0f; // 조준 시 카메라 Z위치
	AttachmentSocketName = TEXT("Scope");
}

void AGJScope::EnableScopeView() // TPS -> FPS (줌 인)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("EnableScopeView FAILED: PlayerController is nullptr"));
		return;
	}

	AGJCharacter* PlayerCharacter = Cast<AGJCharacter>(PlayerController->GetPawn());
	if (PlayerCharacter && PlayerCharacter->SpringArmComp && PlayerCharacter->CameraComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Before Zoom In - Control Rotation: %s"), *PlayerController->GetControlRotation().ToString());

		// 기존 TPS 상태 저장
		StoredControlRotation = PlayerController->GetControlRotation();
		StoredSpringArmRotation = PlayerCharacter->SpringArmComp->GetComponentRotation();
		StoredCameraRelativeLocation = PlayerCharacter->CameraComp->GetRelativeLocation();
		StoredCameraRotation = PlayerCharacter->CameraComp->GetComponentRotation(); // 카메라 회전값도 저장!
		StoredSpringArmLength = PlayerCharacter->SpringArmComp->TargetArmLength;

		// SpringArm을 비활성화하기 전에 현재 회전값과 동기화
		PlayerCharacter->SpringArmComp->SetWorldRotation(StoredControlRotation);

		// 1인칭 모드 전환
		PlayerCharacter->SpringArmComp->TargetArmLength = 0.0f;
		PlayerCharacter->SpringArmComp->bUsePawnControlRotation = false;
		PlayerCharacter->CameraComp->bUsePawnControlRotation = true;

		// FPS 모드에서도 기존 회전값 유지
		PlayerController->SetControlRotation(StoredControlRotation);
		PlayerCharacter->CameraComp->SetWorldRotation(StoredControlRotation);

		// FOV 변경 (줌 인)
		if (PlayerController)
		{
			PlayerController->PlayerCameraManager->SetFOV(ZoomFOV);
		}

		UE_LOG(LogTemp, Error, TEXT("After Zoom In - Control Rotation: %s"), *PlayerController->GetControlRotation().ToString());
	}
}

void AGJScope::DisableScopeView() // FPS -> TPS (줌 아웃)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("DisableScopeView FAILED: PlayerController is nullptr"));
		return;
	}

	AGJCharacter* PlayerCharacter = Cast<AGJCharacter>(PlayerController->GetPawn());
	if (PlayerCharacter && PlayerCharacter->SpringArmComp && PlayerCharacter->CameraComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Before Zoom Out - Control Rotation: %s"), *PlayerController->GetControlRotation().ToString());

		// SpringArm이 다시 활성화되기 전에 FPS 상태에서 변경된 카메라 회전을 반영
		PlayerCharacter->SpringArmComp->SetWorldRotation(StoredCameraRotation);

		// SpringArm 다시 활성화
		PlayerCharacter->SpringArmComp->bUsePawnControlRotation = true;
		PlayerCharacter->CameraComp->bUsePawnControlRotation = false;
		PlayerCharacter->SpringArmComp->TargetArmLength = StoredSpringArmLength;

		// SpringArm과 Camera의 회전 동기화 (Roll 값 틀어짐 방지)
		FRotator NewRotation = StoredCameraRotation;
		NewRotation.Roll = 0.0f; // Roll 값 초기화하여 회전 축이 꼬이지 않도록 설정
		PlayerCharacter->SpringArmComp->SetWorldRotation(NewRotation);
		PlayerCharacter->CameraComp->SetWorldRotation(NewRotation);

		// 카메라의 상대 위치 복원
		PlayerCharacter->CameraComp->SetRelativeLocation(StoredCameraRelativeLocation);

		// FOV 복구
		if (PlayerController)
		{
			PlayerController->PlayerCameraManager->SetFOV(DefaultFOV);
		}

		UE_LOG(LogTemp, Error, TEXT("After Zoom Out - Control Rotation: %s"), *PlayerController->GetControlRotation().ToString());
	}
}

void AGJScope::AttachToGun(AGJBaseGun* Gun)
{
	if (Gun->GetGunType() != EGunType::Rifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGJScope::AttachToGun - This attachment is only for Rifles!"));
		return;
	}

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

void AGJScope::FixZoomInRotation()
{
	// SpringArm이 FPS 전환 후 회전값이 어긋나는 걸 방지
	if (PlayerController)
	{
		PlayerController->SetControlRotation(StoredControlRotation);
	}
}

void AGJScope::FixZoomOutRotation()
{
	// SpringArm이 다시 활성화되면서 틀어진 회전을 다시 원래 값으로 고정하는 함수!
	if (PlayerController)
	{
		PlayerController->SetControlRotation(StoredControlRotation);
		UE_LOG(LogTemp, Error, TEXT("Fixed Zoom Out Rotation: %s"), *StoredControlRotation.ToString());
	}
}
