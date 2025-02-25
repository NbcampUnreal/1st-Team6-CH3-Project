// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/GJGunInterface.h"
#include "GJBaseGun.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class AGJBaseGunAttachment;

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Revolver      UMETA(DisplayName = "Revolver"),
	Rifle       UMETA(DisplayName = "Rifle"),
	RocketLauncher     UMETA(DisplayName = "RocketLauncher"),
	MiniGun      UMETA(DisplayName = "MiniGun")
};


UCLASS()
class GETTOJOB_API AGJBaseGun : public AActor, public IGJGunInterface
{
	GENERATED_BODY()
	
public:	
	AGJBaseGun();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Attachment")
	FName AttachmentSocketName = TEXT("AttachmentSocket");
	UPROPERTY(EditDefaultsOnly, Category = "Socket | Gun")
	FName GunSocketName = TEXT("AttachmentSocket");

	// Reload 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	UAnimMontage* ReloadMontage; // 재장전용 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	bool bIsReloading; // 재장전 여부 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	FTimerHandle ReloadTimerHandle; // 재장전 타이머 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 MagazineCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ReloadSound;

	// 총기 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	EGunType GunType;

	// 획득 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickupGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickupRevolver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickupRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickupRocketLauncher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickupMiniGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float RecoilStrength;


	// 반동 관련 변수
	FTimerHandle RecoilResetTimer;
	float CurrentRecoilStrength = 0.0f;
	float CurrentYawRecoil = 0.0f;
	float InitialRecoilStrength;
	float InitialYawRecoil;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual void Fire() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual void Reload() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Combat")
	virtual bool IsReloading() override;
	
	virtual void Pickup(ACharacter* PlayerCharacter) override;
	virtual void ThrowAway() override;
	
	void EnablePickup();
	void FinishReload();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon Attachment")
	void EquipAttachment(AGJBaseGunAttachment* Attachment);
	UFUNCTION(BlueprintCallable, Category = "Weapon Attachment")
	void RemoveAttachment();

	virtual float GetDamage() override;
	virtual int32 GetCurrentAmmo() const override;
	virtual int32 GetMaxAmmo() const override;

	virtual void BeginPlay() override;

	EGunType GetGunType() const;

	void ApplyRecoil();
	void ResetRecoil();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AActor> ProjectileClass;

	// 총에 부착할 부착물
	UPROPERTY()
	AGJBaseGunAttachment* CurrentAttachment;

	// 부착물 관련 변수
	bool bIsSilenced;
	bool bHasScope;





private:
	bool bCanPickup;
};
