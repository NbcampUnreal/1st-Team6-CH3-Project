#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/GJBaseGun.h"

// Sets default values
AGJBaseGunAttachment::AGJBaseGunAttachment()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

}

void AGJBaseGunAttachment::AttachToGun(AGJBaseGun* Gun)
{
	if (!Gun)
	{
		return;
	}
	AttachedGun = Gun;
	AttachToComponent(
		Gun->GetRootComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("Scope") //TODO: 총에 Scope Socket 만들어줘야 함.
	);
}

void AGJBaseGunAttachment::DetachFromGun()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachedGun = nullptr;
}

// Called when the game starts or when spawned
void AGJBaseGunAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}
