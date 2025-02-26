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

    FDebuffEffect SlowEffect;
    SlowEffect.DebuffType = EDebuffType::Slow;
    SlowEffect.Duration = 5.0f;
    SlowEffect.Intensity = 0.5f;
    SlowEffect.bCanStack = false;

    DebuffEffects.Add(SlowEffect);
}

void AGJTrapActor::BeginPlay()
{
    Super::BeginPlay();
}

void AGJTrapActor::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
    if (Character && Character->DebuffComponent)  // 디버프 컴포넌트 확인
    {
        for (const FDebuffEffect& Debuff : DebuffEffects)
        {
            Character->DebuffComponent->ApplyDebuff(Debuff); // 디버프 적용
        }

        UE_LOG(LogTemp, Warning, TEXT("Trap Activated! Slow Effect Applied"));
    }
}
