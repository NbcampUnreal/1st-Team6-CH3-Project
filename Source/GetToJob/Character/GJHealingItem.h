#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GJHealingItem.generated.h"

class USphereComponent;
class AGJCharacter;

UCLASS()
class GETTOJOB_API AGJHealingItem : public AActor
{
    GENERATED_BODY()

public:
    AGJHealingItem();

protected:

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
    float HealAmount = 25.0f;

    UFUNCTION(BlueprintCallable, Category = "Healing")
    void Pickup(class AGJCharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Healing")
    void Use(AGJCharacter* Character);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
