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

    AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // ��� ȿ�� ���� (Heal)
    if (Debuff.DebuffType == EDebuffType::Heal)
    {
        OwnerCharacter->ModifyHealth(20.0f); // ü�� ȸ��
        UE_LOG(LogTemp, Warning, TEXT("Healing Applied: +20 HP"));
        return;
    }

    // ��ø �Ұ����� ������� �̹� ������ return
    if (!Debuff.bCanStack && ActiveDebuffs.ContainsByPredicate([&Debuff](const FDebuffEffect& Effect)
        {
            return Effect.DebuffType == Debuff.DebuffType;
        }))
    {
        return;
    }

    ActiveDebuffs.Add(Debuff);
    UpdateDebuffs();

    // ����� Ÿ�̸� �߰�
    FTimerHandle& DebuffTimer = DebuffTimers.FindOrAdd(Debuff.DebuffType);

    // ���� ȿ�� (Bleed)�� �������� ü�� ���� ȿ���̹Ƿ� ���� ó��
    if (Debuff.DebuffType == EDebuffType::Bleed)
    {
        ApplyBleedEffect(Debuff.Intensity, 1.0f, Debuff.Duration); // Debuff.Intensity�� ����Ͽ� Ʈ������ ������ �� ����
        return;
    }


    // �Ϲ����� ����� Ÿ�̸� ����
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

// ���� ȿ�� (Bleed) ó��
void UGJDebuffComponent::ApplyBleedEffect(float DamagePerTick, float TickInterval, float Duration)
{
    AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // ���� ���� Ÿ�̸Ӱ� ������ ����
    if (DebuffTimers.Contains(EDebuffType::Bleed))
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimers[EDebuffType::Bleed]);
    }

    // Ÿ�̸ӷ� ���� ȿ�� ����
    FTimerHandle BleedTimer;
    float ElapsedTime = 0.0f;

    FTimerDelegate BleedDelegate;
    BleedDelegate.BindLambda([this, OwnerCharacter, &ElapsedTime, Duration, DamagePerTick, TickInterval]()
        {
            // ü���� 10 �����̸� ���� ��� ����
            if (OwnerCharacter->GetHealth() <= 10.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("Bleed Effect Stopped: Health too low (<= 10)."));
                RemoveDebuff(EDebuffType::Bleed);
                return;
            }

            if (ElapsedTime >= Duration) // ���� ���ӽð��� ������ ����
            {
                UE_LOG(LogTemp, Warning, TEXT("Bleed Effect Ended."));
                RemoveDebuff(EDebuffType::Bleed);
                return;
            }

            OwnerCharacter->ModifyHealth(-DamagePerTick); // ü�� ����
            ElapsedTime += TickInterval;

            UE_LOG(LogTemp, Warning, TEXT("Bleed Effect: -%f HP, Current Health: %f"), DamagePerTick, OwnerCharacter->GetHealth());
        });

    // 1�ʸ��� ���� ȿ�� ����, �� Duration(5��) �� ����
    GetWorld()->GetTimerManager().SetTimer(
        DebuffTimers[EDebuffType::Bleed],
        BleedDelegate,
        TickInterval,
        true // �ݺ� ����
    );

    // 5�� �� ���� ����
    GetWorld()->GetTimerManager().SetTimer(
        BleedTimer,
        FTimerDelegate::CreateLambda([this]()
            {
                RemoveDebuff(EDebuffType::Bleed);
            }),
        Duration, // 5�� �� ����
        false // �� ���� ����
    );
}


// ����� ����
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
    // ��� ����� ����
    ActiveDebuffs.Empty();

    // ��� Ÿ�̸� ����
    for (auto& DebuffTimer : DebuffTimers)
    {
        GetWorld()->GetTimerManager().ClearTimer(DebuffTimer.Value);
    }
    DebuffTimers.Empty();

    UpdateDebuffs(); // �ӵ� ���� �� ������Ʈ
}

// ����� ���� ������Ʈ
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
            SpeedMultiplier *= Debuff.Intensity; // �ӵ� ����
        }
    }

    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->NormalSpeed * SpeedMultiplier;
    OwnerCharacter->SprintSpeed = OwnerCharacter->NormalSpeed * OwnerCharacter->SprintSpeedMultiplier * SpeedMultiplier;
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = OwnerCharacter->CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Updated Speed: Walk = %f, Sprint = %f"),
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed, OwnerCharacter->SprintSpeed);
}
