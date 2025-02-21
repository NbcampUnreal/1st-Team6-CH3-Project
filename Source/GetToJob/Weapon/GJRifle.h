// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJRifle.generated.h"

UCLASS()
class GETTOJOB_API AGJRifle : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	AGJRifle();

	

	virtual void Fire() override;
	virtual void Reload() override;
	void EnableFire();
	void FinishReload();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float TraceRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CoolDownDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadTime; //재장전 시간

	virtual void Pickup(ACharacter* PlayerCharacter) override;
	virtual void ThrowAway() override;


private:
	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;

	UAnimMontage* ReloadMontage; // 재장전용 애니메이션

	bool bIsReloading; // 재장전 여부 확인
	FTimerHandle ReloadTimerHandle; // 재장전 타이머 핸들
};
