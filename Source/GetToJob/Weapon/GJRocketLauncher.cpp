#include "Weapon/GJRocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
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
	bPickRocketLauncher= false;
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
				MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("Rocket"));
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

void AGJRocketLauncher::Pickup(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	// 플레이어 캐릭터가 총을 가지고 있는지 확인
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->CurrentGun)
	{
		return; // 총을 가지고 있으면 줍지 않는다.
	}

	// 총을 플레이어의 오른쪽 손 본에 장착
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("Rocket")
	);

	// 플레이어가 총을 소유
	SetOwner(PlayerCharacter);
	bPickRocketLauncher = false;

	// 캐릭터가 가진 현재 총 = 장착한 총
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}

	// 주운 이후에는 콜리전 제거
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

