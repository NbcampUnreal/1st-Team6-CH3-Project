// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJSilencer.h"
#include "Weapon/GJBaseGun.h"


AGJSilencer::AGJSilencer()
{
	//기본 설정
	NoiseReductionMultilplier = 0.1f;
	RecoilReductionMultiplier = 0.2f;
	DamageMultiplier = 1.3f;
}

void AGJSilencer::AttachToGun(AGJBaseGun* Gun)
{
	if (Gun->GetGunType() != EGunType::Revolver)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGJScope::AttachToGun - This attachment is only for Rifles!"));
		return;
	}
	Super::AttachToGun(Gun);

	if (Gun)
	{
		Gun->NoiseLevel *= NoiseReductionMultilplier;
		Gun->RecoilStrength *= RecoilReductionMultiplier;
		float NewDamage = Gun->GetDamage();
		NewDamage *= DamageMultiplier;
		Gun->SetDamage(NewDamage);
	}
}

void AGJSilencer::DetachFromGun()
{
	if (AttachedGun)
	{
		AttachedGun->NoiseLevel /= NoiseReductionMultilplier;
		AttachedGun->RecoilStrength /= RecoilReductionMultiplier;
		float NewDamage = AttachedGun->GetDamage();
		NewDamage /= DamageMultiplier;
		AttachedGun->SetDamage(NewDamage);
		UE_LOG(LogTemp, Warning, TEXT("Silencer's Effect Off!!"))
	}
	UE_LOG(LogTemp, Warning, TEXT("No AttachedGun!!"))
	Super::DetachFromGun();
}
