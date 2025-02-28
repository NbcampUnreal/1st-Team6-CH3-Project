#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/GJDebuffType.h"
#include "GJTrapActor.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class ETrapEffectType : uint8
{
    Slow UMETA(DisplayName = "Slow"),
    Bleed UMETA(DisplayName = "Bleed"),
    Heal UMETA(DisplayName = "Heal"),
    SpeedBoost UMETA(DisplayName = "Speed Boost")
};

UCLASS()
class GETTOJOB_API AGJTrapActor : public AActor
{
    GENERATED_BODY()

public:
    AGJTrapActor();

    UPROPERTY(EditDefaultsOnly, Category = "Trap")
    TArray<ETrapEffectType> TrapEffects;

protected:
    /*virtual void BeginPlay() override;*/

private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComp;

    TArray<FDebuffEffect> DebuffEffects;

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    bool bHasActivated = false;
};
