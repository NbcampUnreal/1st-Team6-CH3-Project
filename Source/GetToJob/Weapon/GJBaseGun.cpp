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

	// �ݸ��� ������Ʈ ���� �� ����
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(50.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	// �ѱ� �޽� �߰�
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	// ������ �̺�Ʈ�� ���ε� (�ϴ� ���� �� �̺�Ʈ�� �߻��ϴ� ���� �ʱ� ��������)
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

	// �÷��̾� ĳ���Ͱ� ���� ������ �ִ��� Ȯ��
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->CurrentGun)
	{
		return; // ���� ������ ������ ���� �ʴ´�.
	}

	// ���� �÷��̾��� ������ �� ���� ����
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("hand_r")
	);

	// �÷��̾ ���� ����
	SetOwner(PlayerCharacter);


	// ĳ���Ͱ� ���� ���� �� = ������ ��
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}

	// �ֿ� ���Ŀ��� �ݸ��� ����
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

