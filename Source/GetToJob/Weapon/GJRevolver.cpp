#include "Weapon/GJRevolver.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NPC/GJNPC.h"
#include "Particles/ParticleSystemComponent.h"

AGJRevolver::AGJRevolver()
{
	PrimaryActorTick.bCanEverTick = false;


	FireSound = nullptr;
	FireRate = 100.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	TraceRange = 2000.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 2.0f;
	bPickRevolver = false;
	MagazineCount = INT32_MAX;
	bPickupRevolver = false;
	RecoilStrength = 0.3f;


	GunType = EGunType::Revolver;
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
	
	// 카메라 방향 기준으로 라인 트레이스
	if (OwnerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 반동 호출
		ApplyRecoil();

		// 캐릭터가 가진 소켓 위치 가져오기 // TODO 나중에 총구 소켓(Muzzle)을 만들어서 변경
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
		FVector MuzzleLocation = FVector::ZeroVector;

		if (GJCharacter && GJCharacter->GetMesh())
		{
			MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("Revolver"));
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
				//// 적 처치 시 게이지 상승 로직
				//if (HitActor->ActorHasTag(FName("NPC")))
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Hit NPC!"));

				//	AGJNPC* HitEnemy = Cast<AGJNPC>(HitActor);
				//	if (HitEnemy && HitEnemy->bIsDead)
				//	{
				//		UE_LOG(LogTemp, Warning, TEXT("NPC is Dead! Increasing MiniGun Gauge!"));
				//		if (GJCharacter && GJCharacter->MiniGun)
				//		{
				//			GJCharacter->MiniGun->IncreaseGauge(20.0f);
				//		}
				//	}
				//}

				if (HitActor->ActorHasTag(FName("NPC")))
				{
					UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						HitEffect,
						HitResult.ImpactPoint,
						HitResult.ImpactNormal.Rotation()
					);
					if (HitEffect)
					{

						// 3초 후에 이펙트를 제거하는 타이머 설정
						FTimerHandle ExplosionEffectTimer;
						GetWorldTimerManager().SetTimer(
							ExplosionEffectTimer,
							[SpawnedEffect]()
							{
								if (SpawnedEffect)
								{
									SpawnedEffect->DeactivateSystem(); // 이펙트 중지
									SpawnedEffect->DestroyComponent(); // 컴포넌트 삭제
								}
							},
							1.0f,
							false
						);
					}
				}
				
			}
			// 맞췄을 때 디버그 라인
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		}
		else
		{
			// 못 맞췄을 때 디버그 라인
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
	Super::Reload();
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

void AGJRevolver::Pickup(ACharacter* PlayerCharacter)
{
	Super::Pickup(PlayerCharacter);
	bPickupRevolver = true;
	bPickRevolver = true;
}

void AGJRevolver::ThrowAway()
{
	Super::ThrowAway();
	bPickupRevolver = false;
	bPickRevolver = false;
}
