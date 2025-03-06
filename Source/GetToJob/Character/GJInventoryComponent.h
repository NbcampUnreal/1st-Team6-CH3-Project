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

    // 힐링 아이템 관련
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<AGJHealingItem*> HealingItems;

    void AddHealingItem(AGJHealingItem* NewHealingItem);
    void UseHealingItem();

    //힐링 아이템 사용 쿨타임 관련 변수
    bool bCanUseHealingItem = true;
    FTimerHandle HealingCooldownTimer;

    //힐링 아이템 사용 쿨타임 함수
    void ResetHealingCooldown();

    // 무기 슬롯 배열
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<AGJBaseGun*> WeaponSlots;

    // 최대 슬롯 수 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxWeaponSlots = 4;

    // 무기 추가
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddWeapon(AGJBaseGun* NewWeapon);

    // 무기 제거
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveWeapon(AGJBaseGun* WeaponToRemove);

    // 무기 장착
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    AGJBaseGun* EquipWeaponFromSlot(int32 SlotIndex);
};
