#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJShotGun.generated.h"

/**
 * 
 */
UCLASS()
class GETTOJOB_API AGJShotGun : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
    AGJShotGun();

protected:
    virtual void Fire() override;

    // 넉백 힘 (조절 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun")
    float KnockbackForce;

    // 샷건 탄환 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun")
    int32 PelletCount;

    // 원뿔형 발사 각도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun")
    float SpreadAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Own Weapon")
    bool bPickShotGun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float TraceRange;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float CoolDownDelay;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* FireSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UParticleSystem* HitEffect;

    virtual void Pickup(ACharacter* PlayerCharacter) override;

private:
    bool bCanFire;
    FTimerHandle FireCooldownTimer;


    void EnableFire();
    
};
