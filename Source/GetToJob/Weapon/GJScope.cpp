#include "Weapon/GJScope.h"
#include "Weapon/GJBaseGun.h"



AGJScope::AGJScope()
{
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);


	ZoomFOV = 30.0f; // 조준 시 FOV 값
}

// 줌인
void AGJScope::EnableScopeView()
{
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->SetFOV(ZoomFOV);
	}
}

// 줌아웃
void AGJScope::DisableScopeView()
{
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->SetFOV(DefaultFOV);
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
