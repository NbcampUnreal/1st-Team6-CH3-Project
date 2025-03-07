#include "Weapon/GJShotGun.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AGJShotGun::AGJShotGun()
{
    MaxAmmo = 80;
    MagazineCapacity = 40;
    CurrentAmmo = MagazineCapacity;
    PelletCount = 32;       // 기본 탄환 개수
    SpreadAngle = 30.0f;   // 원뿔형 확산 각도
    KnockbackForce = 5000.0f;  // 넉백 강도

    FireSound = nullptr;
    NoiseLevel = 1.0f;
    FireRate = 40.0f;
    CoolDownDelay = 1 / (FireRate / 60);
    TraceRange = 600.0f;
    bCanFire = true;
    bIsReloading = false;
    MaxAmmo = 120;
    MagazineCapacity = 40;
    CurrentAmmo = MagazineCapacity;
    MagazineCount = 100;
    ReloadTime = 3.0f;
    bPickupRifle = false;
    bPickShotGun = false;
    GunType = EGunType::ShotGun;
    RecoilStrength = 1.0f;
}

void AGJShotGun::Fire()
{
    if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot Fire!!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Shotgun Fire!!"));
    CurrentAmmo = CurrentAmmo - 4;

    // 사운드 재생
    if (FireSound)
    {
        float AdjustVolume = 1.0f * NoiseLevel;
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation(), AdjustVolume);
    }

    AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
    if (!OwnerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    ApplyRecoil();

    FVector MuzzleLocation = GunMesh->GetSocketLocation(MuzzleSocketName);

    // 샷건 탄환 퍼짐
    for (int32 i = 0; i < PelletCount; i++)
    {
        FVector ShotDirection = FMath::VRandCone(CameraRotation.Vector(), FMath::DegreesToRadians(SpreadAngle));
        FVector EndLocation = MuzzleLocation + (ShotDirection * TraceRange);
        FVector EndCenterLocation = MuzzleLocation + CameraRotation.Vector() * TraceRange;

        TArray<FHitResult> HitResults;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        Params.AddIgnoredActor(GetOwner());

        bool bHit = GetWorld()->LineTraceMultiByChannel(
            HitResults,
            MuzzleLocation,
            MuzzleLocation + (ShotDirection * TraceRange),
            ECC_Visibility,
            Params
        );

        if (bHit)
        {
            for (const FHitResult& HitResult : HitResults)
            {
                AActor* HitActor = HitResult.GetActor();
                if (HitActor)
                {
                    // 데미지 적용
                    UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, OwnerController, this, nullptr);

                    // 넉백 효과 적용
                    UPrimitiveComponent* HitComp = HitResult.GetComponent();
                    if (HitComp && HitComp->IsSimulatingPhysics())
                    {
                        HitComp->AddImpulse(ShotDirection * KnockbackForce, NAME_None, true);
                    }

                    // 캐릭터라면 LaunchCharacter 사용
                    ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
                    if (HitCharacter && !HitCharacter->ActorHasTag("Boss"))
                    {
                        // 넉백 전에 캐릭터 간 충돌 해제 (시원한 넉백을 위해)
                        HitCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

                        FVector LiftVector = FVector(0.0f, 0.0f, 400.0f);  // 위로 띄우는 힘
                        HitCharacter->LaunchCharacter(LiftVector, true, true);

                        FTimerHandle KnockbackTimer;
                        GetWorld()->GetTimerManager().SetTimer(KnockbackTimer, [EndCenterLocation, HitCharacter, HitResult, this]()
                            {
                                FVector TraceDirection = (EndCenterLocation - HitResult.TraceStart).GetSafeNormal();
                                FVector KnockbackDirection = TraceDirection * KnockbackForce; // 50% 추가

                                // Z축 힘 제한 (너무 위로 뜨지 않도록)
                                KnockbackDirection.Z = FMath::Clamp(KnockbackDirection.Z, 100.0f, 300.0f);

                                HitCharacter->LaunchCharacter(KnockbackDirection, true, true);
                            }, 0.01f, false);

                        // 일정 시간 후 충돌 활성화
                        FTimerHandle ResetCollisionTimer;
                        GetWorld()->GetTimerManager().SetTimer(ResetCollisionTimer, [HitCharacter]()
                            {
                                if (HitCharacter)
                                {
                                    HitCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
                                }
                            }, 0.5f, false); // 0.5초 후 다시 캐릭터 간 충돌 활성화
                    }

                    // 피격 위치에 이펙트 추가
                    if (HitEffect)
                    {
                        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
                    }
                }
                //// 디버그 선 (테스트용)
                //DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);
            }   
        }
    }

    // 발사 속도 제어
    bCanFire = false;
    GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &AGJShotGun::EnableFire, CoolDownDelay, false);
}



void AGJShotGun::Pickup(ACharacter* PlayerCharacter)
{
    Super::Pickup(PlayerCharacter);
    bPickShotGun = true;
}


void AGJShotGun::EnableFire()
{
    bCanFire = true;
}