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
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJRevolver::OnBeginOverlap);

	FireSound = nullptr;
	FireRate = 30.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	TraceRange = 2000.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 3.0f;

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

	// 플레이어 캐릭터가 총을 가지고 있는지 확인
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->CurrentGun)
	{
		return; // 총을 가지고 있으면 줍지 않는다.
	}

	// 총을 플레이어의 오른쪽 손 본에 장착
	AttachToComponent(PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("hand_r")
	);

	// 플레이어가 총을 소유
	SetOwner(PlayerCharacter);


	// 캐릭터가 가진 현재 총 = 장착한 총
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}

	// 주운 이후에는 콜리전 제거
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGJRevolver::Fire()
{
	
	// 재장전 중이거나, 탄이 없을 경우 발사 불가
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Fire!!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	// 탄을 소비
	CurrentAmmo--;

	// 총 소리 재생
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			FireSound,
			GetActorLocation()
		);
	}
	//캐릭터의 컨트롤러에서 시점 정보를 가져오는 함수 
	AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (OwnerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 캐릭터가 가진 소켓 위치 가져오기 // TODO 나중에 총구 소켓(Muzzle)을 만들어서 변경
		AGJCharacter* Character = Cast<AGJCharacter>(GetOwner());
		FVector MuzzleLocation = FVector::ZeroVector;

		if (Character && Character->GetMesh())
		{
			MuzzleLocation = Character->GetMesh()->GetSocketLocation(TEXT("head"));
		}
		else
		{
			// 못 가져올 경우에는 카메라 Location을 사용
			MuzzleLocation = CameraLocation;
		}

		FVector TraceStart = MuzzleLocation;
		FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TraceRange);

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
					CameraRotation.Vector(),
					HitResult,
					OwnerController,
					this,
					nullptr
				);
			}
			// 맞췄을 때 디버그 라인
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		}
		else
		{
			// 못 맞췄을 때 디버르 라인
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f);
		}
	}

	// 사격 속도에 따른 Delay 필요
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		CoolDownTimerHandle,
		this,
		&AGJRevolver::EnableFire,
		CoolDownDelay,
		false
	);

	// 탄창이 돌아가는 Animation이 있으면 좋을 것 같다.
	// 사격 후 반동이 있으면 좋을 것 같다.
}

void AGJRevolver::Reload()
{
	// 재장전을 할 필요가 없을 때
	if (bIsReloading || CurrentAmmo == MaxAmmo)
	{
		return;
	}
	
	bIsReloading = true;

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
		&AGJRevolver::FinishReload,
		ReloadTime,
		false
	);
}

void AGJRevolver::EnableFire()
{
	bCanFire = true;
}

void AGJRevolver::FinishReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}

void AGJRevolver::BeginPlay()
{
	Super::BeginPlay();
}
