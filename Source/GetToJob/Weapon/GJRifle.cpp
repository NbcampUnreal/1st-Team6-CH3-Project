#include "Weapon/GJRifle.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

AGJRifle::AGJRifle()
{
	PrimaryActorTick.bCanEverTick = false;


	FireSound = nullptr;
	FireRate = 600.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	TraceRange = 2000.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 90;
	MagazineCapacity = 30;
	CurrentAmmo = MagazineCapacity;
	MagazineCount = 100;
	ReloadTime = 3.0f;
	bPickupRifle= false;
	bPickRifle = false;
	GunType = EGunType::Rifle;
	RecoilStrength = 0.4f;
}

void AGJRifle::Fire()
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
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * TraceRange);

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
		&AGJRifle::EnableFire,
		CoolDownDelay,
		false
	);
}

void AGJRifle::Reload()
{
	Super::Reload();
}

void AGJRifle::EnableFire()
{
	bCanFire = true;
}


void AGJRifle::BeginPlay()
{
	Super::BeginPlay();
}

void AGJRifle::Pickup(ACharacter* PlayerCharacter)
{
	Super::Pickup(PlayerCharacter);
	bPickupRifle = true;
	bPickRifle = true;
}

void AGJRifle::ThrowAway()
{
	Super::ThrowAway();
	bPickupRifle = false;
	bPickRifle = false; 
}
