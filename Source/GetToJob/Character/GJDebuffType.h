#pragma once

#include "CoreMinimal.h"
#include "GJDebuffType.generated.h"

// 디버프 타입 정의
UENUM(BlueprintType)
enum class EDebuffType : uint8
{
    None        UMETA(DisplayName = "None"),
    Slow        UMETA(DisplayName = "Slow"),
    Bleed      UMETA(DisplayName = "Bleed"),
    Stun        UMETA(DisplayName = "Stun"),
    Blind       UMETA(DisplayName = "Blind"),
    SpeedBoost        UMETA(DisplayName = "SpeedBoost"),
    Heal        UMETA(DisplayName = "Heal")
};

// 디버프 데이터 구조체
USTRUCT(BlueprintType)
struct FDebuffEffect
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDebuffType DebuffType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;  // 지속 시간 (초)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intensity;  // 효과 강도 (예: 속도 감소 비율)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanStack;  // 중첩 가능 여부

    FDebuffEffect()
    {
        DebuffType = EDebuffType::None;
        Duration = 0.f;
        Intensity = 0.f;
        bCanStack = false;
    }
};
