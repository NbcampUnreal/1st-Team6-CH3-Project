#include "GJHealingItem.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Character/GJInventoryComponent.h"

AGJHealingItem::AGJHealingItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // 콜리전 컴포넌트 추가
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetSphereRadius(50.f);
    RootComponent = CollisionComp;

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJHealingItem::OnBeginOverlap);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AGJHealingItem::OnEndOverlap);
}

void AGJHealingItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
    if (Character)
    {
        Character->InteractableHealingItem = this;
    }
}

void AGJHealingItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
    if (Character && Character->InteractableHealingItem == this)
    {
        Character->InteractableHealingItem = nullptr;
    }
}

void AGJHealingItem::Pickup(AGJCharacter* Character)
{
    if (Character && Character->InventoryComponent)
    {
        Character->InventoryComponent->AddHealingItem(this);
        Destroy(); // 아이템 제거
    }
}

void AGJHealingItem::Use(AGJCharacter* Character)
{
    if (Character)
    {
        float NewHealth = FMath::Clamp(Character->GetHealth() + HealAmount, 0.0f, Character->GetMaxHealth());
        Character->SetHealth(NewHealth);
        UE_LOG(LogTemp, Warning, TEXT("Use Healing Item: HealAmount %f"), HealAmount);
    }
}
