#include "Weapon/GJBaseAmmo.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"

// Sets default values
AGJBaseAmmo::AGJBaseAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetSphereRadius(50.f);

    RootComponent = CollisionComp;

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(CollisionComp);

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseAmmo::OnBeginOverlap);
}

void AGJBaseAmmo::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    UE_LOG(LogTemp, Error, TEXT("AGJBaseAmmo::OnBeginOverlap triggered!"));

    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Error, TEXT("Overlapping with Player"));

        AGJCharacter* Character = Cast<AGJCharacter>(OtherActor);
        if (Character && Character->CurrentGun)
        {
            // ���� ������ �ѱ��� Ÿ�԰� ź�� Ÿ���� ��ġ�ϸ� MaxAmmo ����
            if (Character->CurrentGun->GetGunType() == AmmoType)
            {
                UE_LOG(LogTemp, Error, TEXT("Picked up ammo for %s"), *Character->CurrentGun->GetName());
                Character->CurrentGun->RaiseMaxAmmo(AmmoAmount);
                Destroy();  // ź�� ����
            }
        }
    }
}

// Called when the game starts or when spawned
void AGJBaseAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}


