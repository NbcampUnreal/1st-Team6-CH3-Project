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

    // 이미 존재하는 디버프인지 확인하고 중첩 여부 체크
    if (!Debuff.bCanStack && ActiveDebuffs.ContainsByPredicate([&Debuff](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == Debuff.DebuffType;
        }))
    {
        return;
    }

    ActiveDebuffs.Add(Debuff);
    UpdateDebuffs();

    // 디버프별 타이머를 멤버 변수로 관리
    FTimerHandle& DebuffTimer = DebuffTimers.FindOrAdd(Debuff.DebuffType);

    //  `FTimerDelegate`를 사용한 람다식 적용
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

// 디버프 제거
void UGJDebuffComponent::RemoveDebuff(EDebuffType DebuffType)
{
    ActiveDebuffs.RemoveAll([DebuffType](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == DebuffType;
        });

    // 타이머 초기화
    if (DebuffTimers.Contains(DebuffType))
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimers[DebuffType]);
        DebuffTimers.Remove(DebuffType);
    }

    UpdateDebuffs();
}

// 디버프 상태 업데이트 (Slow 디버프 적용)
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
    }

    // 기본 이동 속도 설정 (디버프 적용)
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->NormalSpeed * SpeedMultiplier;

    // 스프린트 속도도 함께 감소 적용
    OwnerCharacter->SprintSpeed = OwnerCharacter->NormalSpeed * OwnerCharacter->SprintSpeedMultiplier * SpeedMultiplier;
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = OwnerCharacter->CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Updated Speed: Walk = %f, Sprint = %f"),
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed, OwnerCharacter->SprintSpeed);
}
