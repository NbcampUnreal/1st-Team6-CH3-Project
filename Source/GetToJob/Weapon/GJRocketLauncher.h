// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJRocketLauncher.generated.h"

UCLASS()
class GETTOJOB_API AGJRocketLauncher : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	AGJRocketLauncher();
	


	

	virtual void Fire() override;
	virtual void Reload() override;
	void EnableFire();
	void FinishReload();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float InitialSpeed; // 로켓의 초기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CoolDownDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickRocketLauncher;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadTime; //재장전 시간

	virtual void Pickup(ACharacter* PlayerCharacter) override;

	virtual void BeginPlay() override;

private:
	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;

	UAnimMontage* ReloadMontage; // 재장전용 애니메이션

	bool bIsReloading; // 재장전 여부 확인
	FTimerHandle ReloadTimerHandle; // 재장전 타이머 핸들

};
