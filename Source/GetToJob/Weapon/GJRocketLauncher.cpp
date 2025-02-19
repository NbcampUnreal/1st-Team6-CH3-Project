#include "Weapon/GJRocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"



AGJRocketLauncher::AGJRocketLauncher()
{
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
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJRocketLauncher::OnBeginOverlap);

	FireSound = nullptr;
	FireRate = 20.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	InitialSpeed = 200.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 4.0f;
}

void AGJRocketLauncher::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AGJRocketLauncher::Fire()
{
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo--;

	if (ProjectileClass)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn)
		{
			FVector CameraLocation = FVector::ZeroVector;
			FRotator CameraRotation = FRotator::ZeroRotator;

			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController)
			{
				OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
			}

			// ĳ���Ͱ� ���� ���� ��ġ �������� // TODO ���߿� �ѱ� ����(Muzzle)�� ���� ����
			AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
			FVector MuzzleLocation = FVector::ZeroVector;

			if (GJCharacter && GJCharacter->GetMesh())
			{
				MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("head"));
			}
			else
			{
				// �� ������ ��쿡�� ī�޶� Location�� ���
				MuzzleLocation = CameraLocation;
			}

			AActor* SpawnedRocket = GetWorld()->SpawnActor<AActor>(
				ProjectileClass,
				MuzzleLocation,
				CameraRotation
			);

			if (SpawnedRocket)
			{
				SpawnedRocket->SetInstigator(OwnerPawn);
				// ������ �ӵ� �ʱ�ȭ
				UProjectileMovementComponent* ProjectileMovement = SpawnedRocket->FindComponentByClass<UProjectileMovementComponent>();
				if (ProjectileMovement)
				{
					ProjectileMovement->Velocity = CameraRotation.Vector() * InitialSpeed;
				}
			}
		}
	}
	// ��� �ӵ��� ���� Delay �ʿ�
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		CoolDownTimerHandle,
		this,
		&AGJRocketLauncher::EnableFire,
		CoolDownDelay,
		false
	);
}

void AGJRocketLauncher::Reload()
{
}

void AGJRocketLauncher::Pickup(ACharacter* PlayerCharacter)
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

void AGJRocketLauncher::EnableFire()
{
	bCanFire = true;
}

void AGJRocketLauncher::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}
