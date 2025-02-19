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
	float ExplosionRadius = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float InitialSpeed; // ������ �ʱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CoolDownDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadTime; //������ �ð�

	

private:
	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;

	UAnimMontage* ReloadMontage; // �������� �ִϸ��̼�

	bool bIsReloading; // ������ ���� Ȯ��
	FTimerHandle ReloadTimerHandle; // ������ Ÿ�̸� �ڵ�

};
