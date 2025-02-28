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

    // ���� ������ ���� ȿ�� ����
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
    if (bHasActivated) return; // �̹� �ߵ��� ��� ����

    AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
    if (Character && Character->DebuffComponent)
    {
        // ���� ȿ�� ����
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
            Character->ModifyHealth(20.0f); // ü�� ȸ��
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

        // Ʈ���� �� ���� �ߵ��ǵ��� ����
        bHasActivated = true;

        // ���� �ð� �� Ʈ�� ���� (��� ���ŵ� ����)
        Destroy();
    }
}

