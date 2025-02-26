#include "Character/GJInventoryComponent.h"
#include "Character/GJCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UGJInventoryComponent::UGJInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGJInventoryComponent::AddHealingItem(AGJHealingItem* NewHealingItem)
{
    if (!NewHealingItem) return;

    HealingItems.Add(NewHealingItem);
    UE_LOG(LogTemp, Warning, TEXT("Healing Item Added to Inventory: %s"), *NewHealingItem->GetName());
}

void UGJInventoryComponent::UseHealingItem()
{
    if (!bCanUseHealingItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Healing items are not yet available! (Cool time applied)"));
        return;
    }

    if (HealingItems.Num() > 0)
    {
        AGJHealingItem* HealingItem = HealingItems[0];

        if (!HealingItem)
        {
            UE_LOG(LogTemp, Error, TEXT("Healing item is nullptr! Skipping use."));
            return;
        }

        // �÷��̾� ĳ���� ��������
        AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            HealingItem->Use(OwnerCharacter);
        }

        //  ���� �������� �޸𸮿��� ���� (Destroy)
        HealingItems.RemoveAt(0);
        HealingItem->Destroy(); // �����ϰ� ����

        UE_LOG(LogTemp, Warning, TEXT("Healing Item Used and Removed from Inventory"));

        //  ���� ������ ��� �� ��Ÿ�� ����
        bCanUseHealingItem = false;
        GetWorld()->GetTimerManager().SetTimer(
            HealingCooldownTimer,
            this,
            &UGJInventoryComponent::ResetHealingCooldown,
            5.0f,
            false
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Healing Items Available"));
    }
}

// ��Ÿ�� ���� �� �ٽ� ��� �����ϰ� ����
void UGJInventoryComponent::ResetHealingCooldown()
{
    bCanUseHealingItem = true;
    UE_LOG(LogTemp, Warning, TEXT("Healing items available!"));
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
    // ���� �ε��� ��ȿ�� üũ
    if (WeaponSlots.IsValidIndex(SlotIndex))
    {
        AGJBaseGun* SelectedWeapon = WeaponSlots[SlotIndex];

        AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // ���� ���� ���� ����
            OwnerCharacter->PreviousWeaponType = OwnerCharacter->CurrentWeaponType;

            // ���� ����
            OwnerCharacter->EquipWeapon(SelectedWeapon);
        }

        UE_LOG(LogTemp, Log, TEXT("Equipped Weapon: %s"), *SelectedWeapon->GetName());
        return SelectedWeapon;
    }

    // �߸��� �ε��� ���� ����
    UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Slot: %d. WeaponSlots length: %d"), SlotIndex, WeaponSlots.Num());
    return nullptr;
}
