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

	TriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerComp"));
	TriggerComp->InitSphereRadius(100.0f);
	TriggerComp->SetupAttachment(CollisionComp);


	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseGunAttachment::OnOverlapBegin);
	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseGunAttachment::OnTriggerOverlap);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &AGJBaseGunAttachment::OnTriggerEndOverlap);

	bPickupAttachment = false;
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
			bPickupAttachment = true;
//			Destroy();
		}

		
	}
}

void AGJBaseGunAttachment::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") && !bPickupAttachment)
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(OtherActor);
		if (GJCharacter)
		{
			SkeletalMeshComp->SetRenderCustomDepth(true);
		}	
	}
}

void AGJBaseGunAttachment::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(OtherActor);
		if (GJCharacter)
		{
			SkeletalMeshComp->SetRenderCustomDepth(false);
		}
	}
}

// Called when the game starts or when spawned
void AGJBaseGunAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}
