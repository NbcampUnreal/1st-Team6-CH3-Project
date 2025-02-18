#include "Weapon/GJRevolver.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"

AGJRevolver::AGJRevolver()
{
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
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJRevolver::OnBeginOverlap);

	FireSound = nullptr;
	CoolDownDelay = 1 / (FireRate / 60);
	TraceRange = 2000.0f;
	bCanFire = true;

}

void AGJRevolver::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AGJRevolver::Pickup(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	// �÷��̾� ĳ���Ͱ� ���� ������ �ִ��� Ȯ��
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	//if (GJCharacter && GJCharacter->CurrentGun)
	//{
	//	return; // ���� ������ ������ ���� �ʴ´�.
	//}

	// ���� �÷��̾��� ������ �� ���� ����
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("hand_r")
	);

	// �÷��̾ ���� ����
	SetOwner(PlayerCharacter);

	// ĳ���Ͱ� ���� ���� �� = ������ ��
	/*if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}*/

	// �ֿ� ���Ŀ��� �ݸ��� ����
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGJRevolver::Fire()
{
	// ������ ���̰ų�, ź�� ���� ��� �߻� �Ұ�
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}
	
	// ź�� �Һ�
	CurrentAmmo--;

	//ĳ������ ��Ʈ�ѷ����� ���� ������ �������� �Լ� 
	AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (OwnerController)
	{
		FVector Location;
		FRotator Rotation;
		OwnerController->GetPlayerViewPoint(Location, Rotation);

		FVector TraceStart = Location;
		FVector TraceEnd = Location + (Rotation.Vector() * TraceRange);
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			ECC_Visibility,
			Params
		);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				UGameplayStatics::ApplyPointDamage(
					HitActor,
					Damage,
					Rotation.Vector(),
					HitResult,
					OwnerController,
					this,
					nullptr
				);
			}
			// ����׿� �� �׸���(���߿� ����)
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 1.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.f);
		}
	}

	// ��� �ӵ��� ���� Delay �ʿ�
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		CoolDownTimerHandle,
		this,
		&AGJRevolver::EnableFire,
		CoolDownDelay,
		false
	);

	// źâ�� ���ư��� Animation�� ������ ���� �� ����.
	// ��� �� �ݵ��� ������ ���� �� ����.
}

void AGJRevolver::EnableFire()
{
	bCanFire = true;
}

void AGJRevolver::BeginPlay()
{
	Super::BeginPlay();
}
