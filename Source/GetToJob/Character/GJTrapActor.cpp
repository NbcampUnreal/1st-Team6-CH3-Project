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
        // 랜덤 효과 선택
        int32 RandomIndex = FMath::RandRange(0, TrapEffects.Num() - 1);
        ETrapEffectType SelectedEffect = TrapEffects[RandomIndex];

        FDebuffEffect AppliedEffect;
        AppliedEffect.Duration = 5.0f;
        AppliedEffect.bCanStack = false;

        switch (SelectedEffect)
        {
        case ETrapEffectType::Slow:
            AppliedEffect.DebuffType = EDebuffType::Slow;
            AppliedEffect.Intensity = 0.5f;
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Slow Effect Applied"));
            break;

        case ETrapEffectType::Bleed:
            AppliedEffect.DebuffType = EDebuffType::Bleed;
            AppliedEffect.Intensity = 0.3f;
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Bleed Effect Applied"));
            break;

        case ETrapEffectType::Heal:
            Character->ModifyHealth(20.0f); // 체력 회복
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Healing Applied"));
            break;

        case ETrapEffectType::SpeedBoost:
            AppliedEffect.DebuffType = EDebuffType::SpeedBoost;
            AppliedEffect.Intensity = 1.5f;
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Speed Boost Applied"));
            break;
        }

        if (SelectedEffect != ETrapEffectType::Heal)
        {
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
        }

        // 트랩이 한 번만 발동되도록 설정
        bHasActivated = true;

        // 일정 시간 후 트랩 제거 (즉시 제거도 가능)
        Destroy();
    }
}

