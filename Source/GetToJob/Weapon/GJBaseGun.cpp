// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJBaseGun.h"

// Sets default values
AGJBaseGun::AGJBaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGJBaseGun::Fire()
{
	if (bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}
	CurrentAmmo--;
}

void AGJBaseGun::Reload()
{
}

bool AGJBaseGun::IsReloading()
{
	return bIsReloading;
}

void AGJBaseGun::EquipAttachment(AActor* Attachment)
{
}

void AGJBaseGun::RemoveAttachment(AActor* Attachment)
{
}

float AGJBaseGun::GetDamage()
{
	return Damage;
}

int32 AGJBaseGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AGJBaseGun::GetMaxAmmo() const
{
	return MaxAmmo;
}

