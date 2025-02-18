// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/GJGunInterface.h"
#include "GJBaseGun.generated.h"

UCLASS()
class GETTOJOB_API AGJBaseGun : public AActor, public IGJGunInterface
{
	GENERATED_BODY()
	
public:	
	AGJBaseGun();


	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual void Fire() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual void Reload() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual bool IsReloading() override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon Attachment")
	void EquipAttachment(AActor* Attachment);
	UFUNCTION(BlueprintCallable, Category = "Weapon Attachment")
	void RemoveAttachment(AActor* Attachment);

	virtual float GetDamage() override;
	virtual int32 GetCurrentAmmo() const override;
	virtual int32 GetMaxAmmo() const override;


protected:
	// 무기의 스탯과 관련된 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Accuracy;
	
	//CoolDownDelay = 1 / (FireRate / 60); // 초당 발사 간격 결정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float AmmoVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 CurrentAmmo;





	float ReloadTime;
	bool bIsReloading;
	TSubclassOf<class AActor> ProjectileClass;




	FTimerHandle ReloadTimerHandle;
	// 부착물 관련 변수
	bool bIsSilenced;
	bool bHasScope;
	




private:


};
