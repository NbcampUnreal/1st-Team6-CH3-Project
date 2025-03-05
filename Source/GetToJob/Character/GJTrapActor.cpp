#include "Character/GJTrapActor.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Character/GJDebuffComponent.h"

AGJTrapActor::AGJTrapActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(50.f);
    RootComponent = CollisionComp;

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJTrapActor::OnBeginOverlap);

    // 적용 가능한 함정 효과 설정
    TrapEffects = { ETrapEffectType::Slow, ETrapEffectType::Bleed, ETrapEffectType::Heal, ETrapEffectType::SpeedBoost };
}

void AGJTrapActor::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bHasActivated) return; // 이미 발동된 경우 무시

    AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
    if (Character && Character->DebuffComponent)
    {
        float RandomChance = FMath::RandRange(0.0f, 100.0f);
        FDebuffEffect AppliedEffect;
        AppliedEffect.Duration = 5.0f;
        AppliedEffect.bCanStack = false;

        if (RandomChance < 10.0f) // 슬로우: 10% 확률
        {
            AppliedEffect.DebuffType = EDebuffType::Slow;
            AppliedEffect.Intensity = 1.5f;
            AppliedEffect.Duration = 3.0f; // 지속 시간 3초
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Slow Effect Applied"));
        }
        else if (RandomChance < 20.0f) // 출혈: 10% 확률
        {
            AppliedEffect.DebuffType = EDebuffType::Bleed;
            AppliedEffect.Intensity = 2.0f; // 틱당 2 데미지
            AppliedEffect.Duration = 6.0f; // 지속 시간 5초
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Bleed Effect Applied"));
        }
        else if (RandomChance < 60.0f) // 회복: 40% 확률
        {
            Character->ModifyHealth(25.0f); // 체력 회복량 25
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Healing Applied"));
        }
        else // 스피드 부스트: 40% 확률
        {
            AppliedEffect.DebuffType = EDebuffType::SpeedBoost;
            AppliedEffect.Intensity = 1.5f;
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Speed Boost Applied"));
        }

        // 트랩이 한 번만 발동되도록 설정
        bHasActivated = true;

        // 일정 시간 후 트랩 제거
        Destroy();
    }
}

