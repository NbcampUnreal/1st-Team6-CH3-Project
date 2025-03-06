#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/GJBaseGun.h"
//#include "Character/GJCharacter.h"
#include "Character/GJHealingItem.h"
#include "GJInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GETTOJOB_API UGJInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGJInventoryComponent();

    // ���� ������ ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<AGJHealingItem*> HealingItems;

    void AddHealingItem(AGJHealingItem* NewHealingItem);
    void UseHealingItem();

    //���� ������ ��� ��Ÿ�� ���� ����
    bool bCanUseHealingItem = true;
    FTimerHandle HealingCooldownTimer;

    //���� ������ ��� ��Ÿ�� �Լ�
    void ResetHealingCooldown();

    // ���� ���� �迭
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<AGJBaseGun*> WeaponSlots;

    // �ִ� ���� �� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxWeaponSlots = 4;

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
