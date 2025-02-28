#include "Weapon/GJElementalRocketMagazine.h"
#include "Weapon/GJRocketLauncher.h"

AGJElementalRocketMagazine::AGJElementalRocketMagazine()
{
	ExtraAmmoCapacity = 4;

	ElementalSequence = { EGJElementalType::Shock, EGJElementalType::Freeze, EGJElementalType::Burn };

	ShotCount = 0;
}

void AGJElementalRocketMagazine::AttachToGun(AGJBaseGun* Gun)
{
	if (!Gun)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToGun - Gun is NULL"));
		return;
	}

	AGJRocketLauncher* RocketLauncher = Cast<AGJRocketLauncher>(Gun);
	if (!RocketLauncher)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToGun - This attachment is for RocketLauncher"));
		return;
	}

	Super::AttachToGun(Gun);

	RocketLauncher->RaiseMaxAmmo(36);
	RocketLauncher->RaiseMagazineCapacity(ExtraAmmoCapacity);
	RocketLauncher->SetCurrentAmmo(RocketLauncher->GetMagazineCapacity());
	RocketLauncher->bHasMagazine = true;
}

void AGJElementalRocketMagazine::DetachFromGun()
{
	if (AttachedLauncher)
	{
		AttachedLauncher->RaiseMaxAmmo(-36);
		AttachedLauncher->RaiseMagazineCapacity(-ExtraAmmoCapacity);
		AttachedLauncher->SetCurrentAmmo(FMath::Min(AttachedLauncher->GetCurrentAmmo(), AttachedLauncher->GetMagazineCapacity()));
	}
	Super::DetachFromGun();
	UE_LOG(LogTemp, Warning, TEXT("Elemental Rocket Magazine Detached"));
}
