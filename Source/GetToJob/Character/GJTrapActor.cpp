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
        float RandomChance = FMath::RandRange(0.0f, 100.0f);
        FDebuffEffect AppliedEffect;
        AppliedEffect.Duration = 5.0f;
        AppliedEffect.bCanStack = false;

        if (RandomChance < 10.0f) // ���ο�: 10% Ȯ��
        {
            AppliedEffect.DebuffType = EDebuffType::Slow;
            AppliedEffect.Intensity = 1.5f;
            AppliedEffect.Duration = 3.0f; // ���� �ð� 3��
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Slow Effect Applied"));
        }
        else if (RandomChance < 20.0f) // ����: 10% Ȯ��
        {
            AppliedEffect.DebuffType = EDebuffType::Bleed;
            AppliedEffect.Intensity = 2.0f; // ƽ�� 2 ������
            AppliedEffect.Duration = 6.0f; // ���� �ð� 5��
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Bleed Effect Applied"));
        }
        else if (RandomChance < 60.0f) // ȸ��: 40% Ȯ��
        {
            Character->ModifyHealth(25.0f); // ü�� ȸ���� 25
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Healing Applied"));
        }
        else // ���ǵ� �ν�Ʈ: 40% Ȯ��
        {
            AppliedEffect.DebuffType = EDebuffType::SpeedBoost;
            AppliedEffect.Intensity = 1.5f;
            Character->DebuffComponent->ApplyDebuff(AppliedEffect);
            UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Speed Boost Applied"));
        }

        // Ʈ���� �� ���� �ߵ��ǵ��� ����
        bHasActivated = true;

        // ���� �ð� �� Ʈ�� ����
        Destroy();
    }
}

