#include "Character/GJInventoryComponent.h"
#include "Character/GJCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UGJInventoryComponent::UGJInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGJInventoryComponent::AddWeapon(AGJBaseGun* NewWeapon)
{
    if (!NewWeapon) return;

    // 중복 체크: 이미 인벤토리에 있는지 확인
    if (WeaponSlots.Contains(NewWeapon))
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon already exists in inventory: %s"), *NewWeapon->GetName());
        return;
    }

    // 슬롯이 가득 차 있으면 오래된 무기를 제거
    if (WeaponSlots.Num() >= MaxWeaponSlots)
    {
        AGJBaseGun* OldWeapon = WeaponSlots[0];
        OldWeapon->ThrowAway();
        WeaponSlots.RemoveAt(0);
    }

    WeaponSlots.Add(NewWeapon);
    UE_LOG(LogTemp, Log, TEXT("Weapon Added: %s"), *NewWeapon->GetName());
}

void UGJInventoryComponent::RemoveWeapon(AGJBaseGun* WeaponToRemove)
{
    if (WeaponToRemove && WeaponSlots.Contains(WeaponToRemove))
    {
        WeaponSlots.Remove(WeaponToRemove);
        UE_LOG(LogTemp, Log, TEXT("Weapon Removed: %s"), *WeaponToRemove->GetName());
    }
}

AGJBaseGun* UGJInventoryComponent::EquipWeaponFromSlot(int32 SlotIndex)
{
    if (WeaponSlots.IsValidIndex(SlotIndex))
    {
        AGJBaseGun* SelectedWeapon = WeaponSlots[SlotIndex];

        AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            OwnerCharacter->EquipWeapon(SelectedWeapon);
        }

        UE_LOG(LogTemp, Log, TEXT("Equipped Weapon: %s"), *SelectedWeapon->GetName());
        return SelectedWeapon;
    }

    UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Slot: %d"), SlotIndex);
    return nullptr;
}