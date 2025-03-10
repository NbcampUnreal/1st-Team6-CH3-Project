#include "GameManager/GJGameInstance.h"
#include "Character/GJCharacter.h"
#include "Weapon/GJBaseGun.h"
#include "Weapon/GJMiniGun.h"
#include "Weapon/GJScope.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/GJRifle.h"

UGJGameInstance::UGJGameInstance()
{
	EnemyTotalKillCount = 0;
	TotalScore = 0;
	OneWaveScore = 0;
	TwoWaveScore = 0;
	ThreeWaveScore = 0;
	BossWaveScore = 0;
	CurrentWaveIndex = 0;
    SavedHealth = 100.0f;
    SavedMaxHealth = 100.0f;
    SavedUltimateGauge = 0.0f;
    SavedCurrentWeaponIndex = -1;
}

void UGJGameInstance::SaveCharacterState(AGJCharacter* Character)
{
    if (!Character) return;

    SavedHealth = Character->GetHealth();
    SavedMaxHealth = Character->GetMaxHealth();

    if (Character->MiniGun)
    {
        SavedUltimateGauge = Character->MiniGun->GetCurrentGauge();
    }

    // 인벤토리에서 무기 저장
    SavedWeapons.Empty();
    SavedAmmoMap.Empty();
    SavedAttachmentMap.Empty();

    for (AGJBaseGun* Weapon : Character->InventoryComponent->WeaponSlots)
    {
        if (Weapon)
        {
            TSubclassOf<AGJBaseGun> WeaponClass = Weapon->GetClass();
            SavedWeapons.Add(WeaponClass);

            // 부착물 저장
            TArray<TSubclassOf<AGJBaseGunAttachment>> AttachmentClasses;
            for (AGJBaseGunAttachment* Attachment : Weapon->Attachments)
            {
                if (Attachment)
                {
                    AttachmentClasses.Add(Attachment->GetClass());
                }
            }
            SavedAttachmentMap.Add(WeaponClass, AttachmentClasses);
        }
    }

    // 현재 장착된 무기 저장
    SavedCurrentWeaponIndex = Character->InventoryComponent->WeaponSlots.Find(Character->CurrentGun);
}


void UGJGameInstance::LoadCharacterState(AGJCharacter* Character)
{
    if (!Character) return;

    Character->SetHealth(SavedHealth);
    Character->SetMaxHealth(SavedMaxHealth);

    if (Character->MiniGun)
    {
        Character->MiniGun->IncreaseGauge(SavedUltimateGauge);
    }

    // 인벤토리 복원
    Character->InventoryComponent->WeaponSlots.Empty();

    for (TSubclassOf<AGJBaseGun> WeaponClass : SavedWeapons)
    {
        AGJBaseGun* NewWeapon = Character->GetWorld()->SpawnActor<AGJBaseGun>(WeaponClass);
        if (NewWeapon)
        {
            Character->InventoryComponent->AddWeapon(NewWeapon);
            NewWeapon->SetOwner(Character);

            // 부착물 복원
            if (SavedAttachmentMap.Contains(WeaponClass))
            {
                TArray<TSubclassOf<AGJBaseGunAttachment>> AttachmentClasses = SavedAttachmentMap[WeaponClass];

                for (TSubclassOf<AGJBaseGunAttachment> AttachmentClass : AttachmentClasses)
                {
                    AGJBaseGunAttachment* NewAttachment = Character->GetWorld()->SpawnActor<AGJBaseGunAttachment>(AttachmentClass);
                    /*if (AGJScope* GJScope = Cast<AGJScope>(NewAttachment))
                    {
                        if (AGJRifle* GJRifle = Cast<AGJRifle>(NewWeapon))
                        {
                            NewWeapon->bHasScope = true;
                        }
                    }*/
                    if (NewAttachment)
                    {
                        NewWeapon->EquipAttachment(NewAttachment);
                    }
                }
            }

            
            
        }
    }

    // 무기 장착 복원
    if (Character->InventoryComponent->WeaponSlots.IsValidIndex(SavedCurrentWeaponIndex))
    {
        Character->EquipWeapon(Character->InventoryComponent->WeaponSlots[SavedCurrentWeaponIndex]);
    }
}

void UGJGameInstance::AddToScore(int amount)
{
	TotalScore += amount;
}

void UGJGameInstance::SaveOneWaveScore(int amount)
{
	OneWaveScore += amount;
}

void UGJGameInstance::SaveTwoWaveScore(int amount)
{
	TwoWaveScore += amount;
}

void UGJGameInstance::SaveThreeWaveScore(int amount)
{
	ThreeWaveScore += amount;
}

void UGJGameInstance::SaveBossScore(int amount)
{
	BossWaveScore += amount;
}

void UGJGameInstance::AddToEnemyKill(int amount)
{
	EnemyTotalKillCount += amount;
}

int32 UGJGameInstance::GetTotalScore()
{
	return TotalScore;
}

int32 UGJGameInstance::GetOneWaveScore()
{
	return OneWaveScore;
}

int32 UGJGameInstance::GetTwoWaveScore()
{
	return TwoWaveScore;
}

int32 UGJGameInstance::GetThreeWaveScore()
{
	return ThreeWaveScore;
}

int32 UGJGameInstance::GetBossScore()
{
	return BossWaveScore;
}

int32 UGJGameInstance::GetEnemyTotalKillCount()
{
	return EnemyTotalKillCount;
}


