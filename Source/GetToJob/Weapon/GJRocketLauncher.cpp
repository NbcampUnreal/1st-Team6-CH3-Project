#include "Weapon/GJRocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"



AGJRocketLauncher::AGJRocketLauncher()
{
	FireSound = nullptr;
	FireRate = 20.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	InitialSpeed = 200.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 4.0f;
}


void AGJRocketLauncher::Fire()
{
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo--;

	if (ProjectileClass)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn)
		{
			FVector CameraLocation = FVector::ZeroVector;
			FRotator CameraRotation = FRotator::ZeroRotator;

			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController)
			{
				OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
			}

			// 캐릭터가 가진 소켓 위치 가져오기 // TODO 나중에 총구 소켓(Muzzle)을 만들어서 변경
			AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
			FVector MuzzleLocation = FVector::ZeroVector;

			if (GJCharacter && GJCharacter->GetMesh())
			{
				MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("head"));
			}
			else
			{
				// 못 가져올 경우에는 카메라 Location을 사용
				MuzzleLocation = CameraLocation;
			}

			AActor* SpawnedRocket = GetWorld()->SpawnActor<AActor>(
				ProjectileClass,
				MuzzleLocation,
				CameraRotation
			);

			if (SpawnedRocket)
			{
				SpawnedRocket->SetInstigator(OwnerPawn);
				// 로켓의 속도 초기화
				UProjectileMovementComponent* ProjectileMovement = SpawnedRocket->FindComponentByClass<UProjectileMovementComponent>();
				if (ProjectileMovement)
				{
					ProjectileMovement->Velocity = CameraRotation.Vector() * InitialSpeed;
				}
			}
		}
	}
	// 사격 속도에 따른 Delay 필요
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		CoolDownTimerHandle,
		this,
		&AGJRocketLauncher::EnableFire,
		CoolDownDelay,
		false
	);
}

void AGJRocketLauncher::Reload()
{

}


void AGJRocketLauncher::EnableFire()
{
	bCanFire = true;
}

void AGJRocketLauncher::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}

