#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/GJBaseGun.h"
#include "GJInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GETTOJOB_API UGJInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGJInventoryComponent();

    // ���� ���� �迭
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<AGJBaseGun*> WeaponSlots;

    // �ִ� ���� �� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxWeaponSlots = 3;

    // ���� �߰�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddWeapon(AGJBaseGun* NewWeapon);

    // ���� ����
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveWeapon(AGJBaseGun* WeaponToRemove);

    // ���� ����
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    AGJBaseGun* EquipWeaponFromSlot(int32 SlotIndex);
};
