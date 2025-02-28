#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/GJDebuffType.h"
#include "GJDebuffComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GETTOJOB_API UGJDebuffComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGJDebuffComponent();

    void ApplyDebuff(const FDebuffEffect& Debuff);
    void RemoveDebuff(EDebuffType DebuffType);
    void RemoveAllDebuffs();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TArray<FDebuffEffect> ActiveDebuffs;

    UPROPERTY()
    bool bCanApplyDebuff = true;

    void UpdateDebuffs();

    TMap<EDebuffType, FTimerHandle> DebuffTimers; // 디버프별 타이머 저장

    void ApplyBleedEffect(float DamagePerTick, float TickInterval, float Duration);
};
