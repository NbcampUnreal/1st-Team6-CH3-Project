#include "Weapon/GJBaseGun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/GJCharacter.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/GJScope.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGJBaseGun::AGJBaseGun()
{
	PrimaryActorTick.bCanEverTick = false;

	// 콜리전 컴포넌트 생성 및 설정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(50.f);
	
	

	// GunMesh 설정
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMesh;
	CollisionComp->SetupAttachment(RootComponent);

	//// 플레이어가 총을 들고 있지 않을 때만 충돌 비활성화
	//GunMesh->SetSimulatePhysics(false);
	//GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GunMesh->SetCollisionObjectType(ECC_WorldDynamic);


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
	bCanPickup = true;
	MagazineCount = 3;
}

void AGJBaseGun::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") && bCanPickup)
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(OtherActor);
		if (GJCharacter)
		{


			Pickup(GJCharacter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Pickup!!"));
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
	// 재장전을 할 필요가 없을 때
	if (bIsReloading || MagazineCount <= 0)
	{
		return;
	}

	bIsReloading = true;
	MagazineCount--;

	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ReloadSound,
			GetActorLocation()
		);
	}

	// 애님 몽타주 실행 (TODO: add Anim Montage)
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
	if (GJCharacter && ReloadMontage && GJCharacter)
	{
		GJCharacter->GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
	GetWorldTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&AGJBaseGun::FinishReload,
		ReloadTime,
		false
	);
}

bool AGJBaseGun::IsReloading()
{
	return bIsReloading;
}

void AGJBaseGun::Pickup(ACharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Player!!"));
		return;
	}
	// 플레이어 캐릭터가 총을 가지고 있는지 확인
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->CurrentGun)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Gun Or No Cast!!"));
		return; // 총을 가지고 있으면 줍지 않는다.
	}

	// 총을 플레이어의 오른쪽 손 본에 장착
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		GunSocketName
	);

	// 플레이어가 총을 소유
	SetOwner(PlayerCharacter);
	bPickupGun = true;


	// 캐릭터가 가진 현재 총 = 장착한 총
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No CurrentGun!!"));
	}

	// 주운 이후에는 콜리전 제거
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGJBaseGun::ThrowAway()
{
	if (!bPickupGun)
	{
		UE_LOG(LogTemp, Error, TEXT("총 없음"));
		return;
	}

	// 소켓에서 분리 (부모 컴포넌트를 떼어내야 함)
	GunMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	bCanPickup = false;

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
	if (PlayerCharacter)
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
		if (GJCharacter && GJCharacter->CurrentGun == this)
		{
			GJCharacter->CurrentGun = nullptr;
		}
	}

	//// 물리 활성화
	//GunMesh->SetSimulatePhysics(true);
	//GunMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);



	// 총을 약간 앞쪽으로 떨어뜨리기
	FVector ThrowDirection = PlayerCharacter ? PlayerCharacter->GetActorForwardVector() : FVector::ForwardVector;
	SetActorLocation(GetActorLocation() + ThrowDirection * 100.0f);



	FTimerHandle TimerHandle_EnablePickup;
	// 2초 후 충돌 다시 활성화
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnablePickup, this, &AGJBaseGun::EnablePickup, 2.0f, false);

	SetOwner(nullptr);
	bPickupGun = false;

	UE_LOG(LogTemp, Warning, TEXT("ThrowAway 완료 - 2초 후 다시 줍기 가능"));


}

void AGJBaseGun::EnablePickup()
{
	// 다시 충돌 활성화하여 주울 수 있도록 함
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);



	UE_LOG(LogTemp, Warning, TEXT("무기 줍기 가능"));
	bCanPickup = true;





}

void AGJBaseGun::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}

void AGJBaseGun::EquipAttachment(AGJBaseGunAttachment* Attachment)
{
	if (!Attachment)
	{
		return;
	}
	RemoveAttachment(); // 기존 부착물이 있다면 제거

	CurrentAttachment = Attachment;
	CurrentAttachment->AttachToGun(this);

	// 부착물을 부착
	CurrentAttachment->AttachToComponent(
		GunMesh,
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		AttachmentSocketName
	);

	if (AGJScope* GJScope = Cast<AGJScope>(Attachment))
	{
		bHasScope = true;
		GJScope->EnableScopeView();
	}


}

void AGJBaseGun::RemoveAttachment()
{
	if (!CurrentAttachment) return;

	if (AGJScope* Scope = Cast<AGJScope>(CurrentAttachment))
	{
		bHasScope = false;
	}

	// 부착물에서 메시를 떼기
	CurrentAttachment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	CurrentAttachment->DetachFromGun();
	CurrentAttachment = nullptr;
	
	
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

void AGJBaseGun::BeginPlay()
{
	Super::BeginPlay();

}

EGunType AGJBaseGun::GetGunType() const
{
	 return GunType; 
}

