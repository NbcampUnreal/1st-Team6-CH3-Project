﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJRevolver.generated.h"

class GJCharacter;

UCLASS()
class GETTOJOB_API AGJRevolver : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	AGJRevolver();


	
	virtual void Fire() override;
	virtual void Reload() override;
	void EnableFire();
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickRevolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float TraceRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CoolDownDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* HitEffect;

	virtual void Pickup(ACharacter* PlayerCharacter) override;
	virtual void ThrowAway() override;
	

private:
	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;

	UAnimMontage* ReloadMontage; // 재장전용 애니메이션

	FTimerHandle ReloadTimerHandle; // 재장전 타이머 핸들
};
