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

    // �ߺ� üũ: �̹� �κ��丮�� �ִ��� Ȯ��
    if (WeaponSlots.Contains(NewWeapon))
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon already exists in inventory: %s"), *NewWeapon->GetName());
        return;
    }

    // ������ ���� �� ������ ������ ���⸦ ����
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