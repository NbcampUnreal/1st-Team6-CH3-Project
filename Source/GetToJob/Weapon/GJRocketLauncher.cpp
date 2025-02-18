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

			// TODO ������ ���������� ���ư����� ProjectileMovementComponent ����
			// ARocketProjectile::InitVelocity(SpawnRotation.Vector() * InitialSpped);

			// TODO ������ ���� ������ ����, ������ ó��
		}
	}
}
