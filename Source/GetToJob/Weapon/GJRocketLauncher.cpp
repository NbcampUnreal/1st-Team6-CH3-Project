#include "Weapon/GJRocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AGJRocketLauncher::Fire()
{
	if (bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo--;

	if (ProjectileClass)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn)
		{
			FVector SpawnLocation;
			FRotator SpawnRotation;
			OwnerPawn->GetController()->GetPlayerViewPoint(SpawnLocation, SpawnRotation);

			AActor* SpawnedRocket = GetWorld()->SpawnActor<AActor>(
				ProjectileClass,
				SpawnLocation,
				SpawnRotation
			);
			if (SpawnedRocket)
			{
				SpawnedRocket->SetInstigator(OwnerPawn);
			}

			// TODO 로켓이 물리적으로 날아가도록 ProjectileMovementComponent 설정
			// ARocketProjectile::InitVelocity(SpawnRotation.Vector() * InitialSpped);

			// TODO 로켓의 폭발 데미지 설정, 데미지 처리
		}
	}
}
