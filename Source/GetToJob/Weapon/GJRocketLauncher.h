// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "Weapon/EGJElementalType.h"
#include "GJRocketLauncher.generated.h"

UCLASS()
class GETTOJOB_API AGJRocketLauncher : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	AGJRocketLauncher();
	
	// 부착된 속성 탄환 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	class AGJElementalRocketMagazine* EquippedElementalMagazine;

	bool bHasMagazine;

	virtual void Fire() override;
	virtual void Reload() override;
	void EnableFire();
	
	EGJElementalType GetNextRocketElement();


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


	virtual void Pickup(ACharacter* PlayerCharacter) override;
	virtual void ThrowAway() override;

	virtual void BeginPlay() override;

private:
	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;
	

	UAnimMontage* ReloadMontage; // 재장전용 애니메이션

	FTimerHandle ReloadTimerHandle; // 재장전 타이머 핸들

};
