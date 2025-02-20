// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJBaseGun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/GJCharacter.h"


// Sets default values
AGJBaseGun::AGJBaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 콜리전 컴포넌트 생성 및 설정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(50.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	// 총기 메쉬 추가
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	// 오버랩 이벤트를 바인딩 (일단 접촉 시 이벤트가 발생하는 것을 초기 설정으로)
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseGun::OnBeginOverlap);


	Damage = 100.0f;
	Accuracy = 100.0f;
	AmmoVelocity = 100.0f;
	FireRate = 200.0f;

	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;


	bIsSilenced = false;
	bHasScope = false;
	ReloadTime = 3.0f;
	bIsReloading = false;
	bPickupGun = false;
}

void AGJBaseGun::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
		if (PlayerCharacter)
		{
			Pickup(PlayerCharacter);
		}
	}
}

void AGJBaseGun::Fire()
{
	if (bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}
	CurrentAmmo--;
}

void AGJBaseGun::Reload()
{
}

bool AGJBaseGun::IsReloading()
{
	return bIsReloading;
}

void AGJBaseGun::Pickup(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	// 플레이어 캐릭터가 총을 가지고 있는지 확인
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->CurrentGun)
	{
		return; // 총을 가지고 있으면 줍지 않는다.
	}

	// 총을 플레이어의 오른쪽 손 본에 장착
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("Gun")
	);

	// 플레이어가 총을 소유
	SetOwner(PlayerCharacter);
	bPickupGun = true;


	// 캐릭터가 가진 현재 총 = 장착한 총
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}

	// 주운 이후에는 콜리전 제거
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGJBaseGun::EquipAttachment(AActor* Attachment)
{
}

void AGJBaseGun::RemoveAttachment(AActor* Attachment)
{
}

float AGJBaseGun::GetDamage()
{
	return Damage;
}

int32 AGJBaseGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AGJBaseGun::GetMaxAmmo() const
{
	return MaxAmmo;
}

