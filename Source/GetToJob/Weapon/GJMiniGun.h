#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJMiniGun.generated.h"


UCLASS()
class GETTOJOB_API AGJMiniGun : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	AGJMiniGun();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
	bool bPickMiniGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float TraceRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float CoolDownDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultra")
	bool bUltraIsReady;


	UFUNCTION()
	void ActivateMiniGun();
	UFUNCTION()
	void DeactivateMiniGun();
	UFUNCTION()
	void IncreaseGauge(float Amount);

	virtual void Fire() override;
	virtual void Reload() override {}; // 탄약 무제한이므로 재장전 불가
	
	bool IsMiniGunActive() const;
	void EnableFire();

protected:
	virtual void BeginPlay() override;


private:
	void StartDeactivationTimer();
	void ResetGauge();

	UPROPERTY(EditAnywhere, Category = "MiniGun")
	float MaxGauge; // 최대 게이지 값

	UPROPERTY(EditAnywhere, Category = "MiniGun")
	float CurrentGauge; // 현재 게이지 값

	UPROPERTY(EditAnywhere, Category = "MiniGun")
	float MiniGunDuration; // 미니건 활성화 시간

	FTimerHandle MiniGunTimerHandle;

	FTimerHandle CoolDownTimerHandle;
	bool bCanFire;
};
