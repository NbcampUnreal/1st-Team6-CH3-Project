#include "Character/GJDebuffComponent.h"
#include "Character/GJCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UGJDebuffComponent::UGJDebuffComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGJDebuffComponent::BeginPlay()
{
    Super::BeginPlay();
}

// 디버프 적용
void UGJDebuffComponent::ApplyDebuff(const FDebuffEffect& Debuff)
{
    if (!bCanApplyDebuff) return;

    AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // 즉시 효과 적용 (Heal)
    if (Debuff.DebuffType == EDebuffType::Heal)
    {
        OwnerCharacter->ModifyHealth(20.0f); // 체력 회복
        UE_LOG(LogTemp, Warning, TEXT("Healing Applied: +20 HP"));
        return;
    }

    // 중첩 불가능한 디버프가 이미 있으면 return
    if (!Debuff.bCanStack && ActiveDebuffs.ContainsByPredicate([&Debuff](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == Debuff.DebuffType;
        }))
    {
        return;
    }

    ActiveDebuffs.Add(Debuff);
    UpdateDebuffs();

    // 디버프 타이머 추가
    FTimerHandle& DebuffTimer = DebuffTimers.FindOrAdd(Debuff.DebuffType);

    // 출혈 효과 (Bleed)는 지속적인 체력 감소 효과이므로 따로 처리
    if (Debuff.DebuffType == EDebuffType::Bleed)
    {
        ApplyBleedEffect(Debuff.Intensity, 1.0f, Debuff.Duration); // Debuff.Intensity를 사용하여 트랩에서 설정한 값 적용
        return;
    }


    // 일반적인 디버프 타이머 설정
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([this, Debuff]()
        {
            RemoveDebuff(Debuff.DebuffType);
        });

    GetWorld()->GetTimerManager().SetTimer(
        DebuffTimer,
        TimerDelegate,
        Debuff.Duration,
        false
    );
}

// 출혈 효과 (Bleed) 처리
void UGJDebuffComponent::ApplyBleedEffect(float DamagePerTick, float TickInterval, float Duration)
{
    AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // 기존 출혈 타이머가 있으면 제거
    if (DebuffTimers.Contains(EDebuffType::Bleed))
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimers[EDebuffType::Bleed]);
    }

    // 타이머로 출혈 효과 적용
    FTimerHandle BleedTimer;
    float ElapsedTime = 0.0f;

    FTimerDelegate BleedDelegate;
    BleedDelegate.BindLambda([this, OwnerCharacter, &ElapsedTime, Duration, DamagePerTick, TickInterval]()
        {
            // 체력이 10 이하이면 출혈 즉시 해제
            if (OwnerCharacter->GetHealth() <= 10.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("Bleed Effect Stopped: Health too low (<= 10)."));
                RemoveDebuff(EDebuffType::Bleed);
                return;
            }

            if (ElapsedTime >= Duration) // 출혈 지속시간이 지나면 종료
            {
                UE_LOG(LogTemp, Warning, TEXT("Bleed Effect Ended."));
                RemoveDebuff(EDebuffType::Bleed);
                return;
            }

            OwnerCharacter->ModifyHealth(-DamagePerTick); // 체력 감소
            ElapsedTime += TickInterval;

            UE_LOG(LogTemp, Warning, TEXT("Bleed Effect: -%f HP, Current Health: %f"), DamagePerTick, OwnerCharacter->GetHealth());
        });

    // 1초마다 출혈 효과 적용, 총 Duration(5초) 후 종료
    GetWorld()->GetTimerManager().SetTimer(
        DebuffTimers[EDebuffType::Bleed],
        BleedDelegate,
        TickInterval,
        true // 반복 실행
    );

    // 5초 후 출혈 제거
    GetWorld()->GetTimerManager().SetTimer(
        BleedTimer,
        FTimerDelegate::CreateLambda([this]()
            {
                RemoveDebuff(EDebuffType::Bleed);
            }),
        Duration, // 5초 후 제거
        false // 한 번만 실행
    );
}


// 디버프 제거
void UGJDebuffComponent::RemoveDebuff(EDebuffType DebuffType)
{
    ActiveDebuffs.RemoveAll([DebuffType](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == DebuffType;
        });

    if (DebuffTimers.Contains(DebuffType))
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimers[DebuffType]);
        DebuffTimers.Remove(DebuffType);
    }

    UpdateDebuffs();
}

void UGJDebuffComponent::RemoveAllDebuffs()
{
    // 모든 디버프 제거
    ActiveDebuffs.Empty();

    // 모든 타이머 정리
    for (auto& DebuffTimer : DebuffTimers)
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimer.Value);
    }
    DebuffTimers.Empty();

    UpdateDebuffs(); // 속도 복원 등 업데이트
}

// 디버프 상태 업데이트
void UGJDebuffComponent::UpdateDebuffs()
{
    AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
    if (!MovementComp) return;

    float SpeedMultiplier = 1.0f;

    for (const FDebuffEffect& Debuff : ActiveDebuffs)
    {
        if (Debuff.DebuffType == EDebuffType::Slow)
        {
            SpeedMultiplier *= Debuff.Intensity;
        }
        else if (Debuff.DebuffType == EDebuffType::SpeedBoost)
        {
            SpeedMultiplier *= Debuff.Intensity; // 속도 증가
        }
    }

    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->NormalSpeed * SpeedMultiplier;
    OwnerCharacter->SprintSpeed = OwnerCharacter->NormalSpeed * OwnerCharacter->SprintSpeedMultiplier * SpeedMultiplier;
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = OwnerCharacter->CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Updated Speed: Walk = %f, Sprint = %f"),
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed, OwnerCharacter->SprintSpeed);
}
