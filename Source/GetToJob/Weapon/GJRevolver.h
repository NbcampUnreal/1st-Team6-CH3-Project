// Fill out your copyright notice in the Description page of Project Settings.

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
	void FinishReload();
	

protected:
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float TraceRange;
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
