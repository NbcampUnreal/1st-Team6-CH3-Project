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

			// ĳ���Ͱ� ���� ���� ��ġ �������� // TODO ���߿� �ѱ� ����(Muzzle)�� ���� ����
			AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
			FVector MuzzleLocation = FVector::ZeroVector;

			if (GJCharacter && GJCharacter->GetMesh())
			{
				MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("head"));
			}
			else
			{
				// �� ������ ��쿡�� ī�޶� Location�� ���
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
				// ������ �ӵ� �ʱ�ȭ
				UProjectileMovementComponent* ProjectileMovement = SpawnedRocket->FindComponentByClass<UProjectileMovementComponent>();
				if (ProjectileMovement)
				{
					ProjectileMovement->Velocity = CameraRotation.Vector() * InitialSpeed;
				}
			}
		}
	}
	// ��� �ӵ��� ���� Delay �ʿ�
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

