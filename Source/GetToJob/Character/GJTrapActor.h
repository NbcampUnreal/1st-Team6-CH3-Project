#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/GJDebuffType.h"
#include "GJTrapActor.generated.h"

class USphereComponent;

UCLASS()
class GETTOJOB_API AGJTrapActor : public AActor
{
    GENERATED_BODY()

public:
    AGJTrapActor();

protected:
    virtual void BeginPlay() override;

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
};
