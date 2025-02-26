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

// ����� ����
void UGJDebuffComponent::ApplyDebuff(const FDebuffEffect& Debuff)
{
    if (!bCanApplyDebuff) return;

    // �̹� �����ϴ� ��������� Ȯ���ϰ� ��ø ���� üũ
    if (!Debuff.bCanStack && ActiveDebuffs.ContainsByPredicate([&Debuff](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == Debuff.DebuffType;
        }))
    {
        return;
    }

    ActiveDebuffs.Add(Debuff);
    UpdateDebuffs();

    // ������� Ÿ�̸Ӹ� ��� ������ ����
    FTimerHandle& DebuffTimer = DebuffTimers.FindOrAdd(Debuff.DebuffType);

    //  `FTimerDelegate`�� ����� ���ٽ� ����
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

// ����� ����
void UGJDebuffComponent::RemoveDebuff(EDebuffType DebuffType)
{
    ActiveDebuffs.RemoveAll([DebuffType](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == DebuffType;
        });

    // Ÿ�̸� �ʱ�ȭ
    if (DebuffTimers.Contains(DebuffType))
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimers[DebuffType]);
        DebuffTimers.Remove(DebuffType);
    }

    UpdateDebuffs();
}

// ����� ���� ������Ʈ (Slow ����� ����)
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

    // �⺻ �̵� �ӵ� ���� (����� ����)
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->NormalSpeed * SpeedMultiplier;

    // ������Ʈ �ӵ��� �Բ� ���� ����
    OwnerCharacter->SprintSpeed = OwnerCharacter->NormalSpeed * OwnerCharacter->SprintSpeedMultiplier * SpeedMultiplier;
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = OwnerCharacter->CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Updated Speed: Walk = %f, Sprint = %f"),
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed, OwnerCharacter->SprintSpeed);
}
