#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/GJBaseGun.h"
#include "Character/GJCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AGJBaseGunAttachment::AGJBaseGunAttachment()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComp->SetupAttachment(RootComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SkeletalMeshComp->SetupAttachment(CollisionComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseGunAttachment::OnOverlapBegin);
}

void AGJBaseGunAttachment::AttachToGun(AGJBaseGun* Gun)
{
	if (!Gun)
	{
		return;
	}
	//AttachedGun = Gun;
	//AttachToComponent(
	//	Gun->GetRootComponent(),
	//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//	TEXT("Scope") //TODO: 총에 Scope Socket 만들어줘야 함.
	//);
}

void AGJBaseGunAttachment::DetachFromGun()
{
	AttachedGun = nullptr;
}

void AGJBaseGunAttachment::OnOverlapBegin(UPrimitiveComponent* OverlapeedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin triggered"));

	AGJCharacter* GJPlayerCharacter = Cast<AGJCharacter>(OtherActor);
	if (GJPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter detected"));
		
		if (GJPlayerCharacter->CurrentGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerGun detected"));
			AGJBaseGun* PlayerGun = GJPlayerCharacter->CurrentGun;
			PlayerGun->EquipAttachment(this);
			UE_LOG(LogTemp, Warning, TEXT("Attachment detected"));
//			Destroy();
		}

		
	}
}

// Called when the game starts or when spawned
void AGJBaseGunAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}
