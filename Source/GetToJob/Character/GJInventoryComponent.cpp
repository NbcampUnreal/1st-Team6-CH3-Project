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

        // 플레이어 캐릭터 가져오기
        AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            HealingItem->Use(OwnerCharacter);
        }

        //  힐링 아이템을 메모리에서 제거 (Destroy)
        HealingItems.RemoveAt(0);
        HealingItem->Destroy(); // 안전하게 삭제

        UE_LOG(LogTemp, Warning, TEXT("Healing Item Used and Removed from Inventory"));

        //  힐링 아이템 사용 후 쿨타임 적용
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

// 쿨타임 종료 후 다시 사용 가능하게 설정
void UGJInventoryComponent::ResetHealingCooldown()
{
    bCanUseHealingItem = true;
    UE_LOG(LogTemp, Warning, TEXT("Healing items available!"));
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
    // 슬롯 인덱스 유효성 체크
    if (WeaponSlots.IsValidIndex(SlotIndex))
    {
        AGJBaseGun* SelectedWeapon = WeaponSlots[SlotIndex];

        AGJCharacter* OwnerCharacter = Cast<AGJCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // 이전 무기 상태 저장
            OwnerCharacter->PreviousWeaponType = OwnerCharacter->CurrentWeaponType;

            // 무기 장착
            OwnerCharacter->EquipWeapon(SelectedWeapon);
        }

        UE_LOG(LogTemp, Log, TEXT("Equipped Weapon: %s"), *SelectedWeapon->GetName());
        return SelectedWeapon;
    }

    // 잘못된 인덱스 접근 방지
    UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Slot: %d. WeaponSlots length: %d"), SlotIndex, WeaponSlots.Num());
    return nullptr;
}
