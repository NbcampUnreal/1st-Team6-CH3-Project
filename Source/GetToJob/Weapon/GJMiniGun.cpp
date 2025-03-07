#include "Weapon/GJMiniGun.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/GJBaseGunAttachment.h"
//#include "NiagaraFunctionLibrary.h"
//#include "NiagaraComponent.h" // 나이아가라 추가

AGJMiniGun::AGJMiniGun()
{
	// 총기 관련
	Damage = 100.0f;
	FireRate = 1000.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	MaxAmmo = INT32_MAX;
	MagazineCapacity = INT32_MAX;
	CurrentAmmo = MagazineCapacity;
	NoiseLevel = 1.0f;
	FireSound = nullptr;
	TraceRange = 6000.0f;
	bCanFire = true;
	bPickMiniGun = false;
	GunType = EGunType::MiniGun;

	// 게이지 관련
	CurrentGauge = 0.0f;
	MaxGauge = 100.0f;
	MiniGunDuration = 30.0f;

	// 처음에는 숨김 상태로 시작
	GunMesh->SetVisibility(false);
	// 주울 일 없으니까 Collision은 없앤다.
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bUltraIsReady = false;
}

void AGJMiniGun::ActivateMiniGun()
{
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
	if (!GJCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("MiniGun activation failed: No valid Owner"));
		return;
	}

	// 기존 무기 비활성화
	if (GJCharacter->CurrentGun && GJCharacter)
	{
		
		GJCharacter->CurrentGun->SetActorHiddenInGame(true);
		GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (GJCharacter->CurrentGun->Attachments.Num() > 0)
		{
			TArray<USkeletalMeshComponent*> MeshComponents;
			GJCharacter->CurrentGun->Attachments[0]->GetComponents<USkeletalMeshComponent>(MeshComponents);

			for (USkeletalMeshComponent* MeshComp : MeshComponents)
			{
				if (MeshComp)
				{
					MeshComp->SetVisibility(false);
				}
			}
		}
	}

	GunMesh->SetVisibility(true);
	GJCharacter->CurrentGun = this;
	bPickupMiniGun = true;

	GJCharacter->CurrentWeaponType = EWeaponType::MiniGun;

	UE_LOG(LogTemp, Warning, TEXT("MiniGun Activated!"));

	// 제한 시간이 지나면 미니건 비활성화
	StartDeactivationTimer();
}

void AGJMiniGun::DeactivateMiniGun()
{
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
	if (GJCharacter && GJCharacter->CurrentGun == this)
	{
		GunMesh->SetVisibility(false);
	}

	// 기존 무기 다시 보이게 하기 (큇슬롯 로직 구현 시 구현하자)
	GJCharacter->CurrentGun = nullptr; // 추후 수정
	GJCharacter->CurrentWeaponType = EWeaponType::None;
	UE_LOG(LogTemp, Warning, TEXT("MiniGun Deactivated!!"));
	bPickupMiniGun = false;
	/*if (GJCharacter->CurrentGun && GJCharacter->CurrentGun->Attachments.Num() > 0)
	{
		TArray<USkeletalMeshComponent*> MeshComponents;
		GJCharacter->CurrentGun->Attachments[0]->GetComponents<USkeletalMeshComponent>(MeshComponents);

		for (USkeletalMeshComponent* MeshComp : MeshComponents)
		{
			if (MeshComp)
			{
				MeshComp->SetVisibility(true);
			}
		}
	}*/

	ResetGauge();
	bUltraIsReady = false;
}

void AGJMiniGun::IncreaseGauge(float Amount)
{
	CurrentGauge += Amount;
	if (CurrentGauge >= MaxGauge)
	{
		CurrentGauge = MaxGauge;
		bUltraIsReady = true;
	}
}

void AGJMiniGun::Fire()
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
		float AdjustVolume = 1.0f * NoiseLevel;
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			FireSound,
			GetActorLocation(),
			AdjustVolume
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
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
		FVector MuzzleLocation = FVector::ZeroVector;

		if (GJCharacter && GJCharacter->GetMesh())
		{
			MuzzleLocation = GunMesh->GetSocketLocation(TEXT("MiniGun"));
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
			TraceEnd = HitResult.ImpactPoint;


			// 레이저 연출: 굵은 디버그 실린더 추가
			DrawDebugCylinder(GetWorld(), TraceStart, TraceEnd, 7.0f, 12, FColor::Red, false, 0.1f, 0, 5.0f);
		}
		else
		{
			// 레이저 연출: 굵은 디버그 실린더 추가
			DrawDebugCylinder(GetWorld(), TraceStart, TraceEnd, 7.0f, 12, FColor::Red, false, 0.1f, 0, 5.0f);
		}

		// Niagara 기반 Sci-Fi 레이저 효과 적용
		//if (LaserBeamNiagara)
		//{
		//	UNiagaraComponent* LaserEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		//		GetWorld(),
		//		LaserBeamNiagara,
		//		TraceStart
		//	);

		//	if (LaserEffect)
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("TraceStart: %s, TraceEnd: %s"),
		//			*TraceStart.ToString(), *TraceEnd.ToString());

		//		// Beam의 끝 위치 설정
		//		LaserEffect->SetVectorParameter(TEXT("BeamEnd"), TraceEnd);

		//		// Beam의 시작 위치 설정
		//		LaserEffect->SetVectorParameter(TEXT("BeamStart"), TraceStart);	

		//		// 실행 상태 확인
		//		LaserEffect->SetAutoActivate(true);
		//		LaserEffect->ActivateSystem();
		//		LaserEffect->ReinitializeSystem(); // 강제 업데이트

		//		UE_LOG(LogTemp, Error, TEXT("Beam Render working!"));
		//	}
		//}
	}
	// 사격 속도에 따른 Delay 필요
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		CoolDownTimerHandle,
		this,
		&AGJMiniGun::EnableFire,
		CoolDownDelay,
		false
	);
}

bool AGJMiniGun::IsMiniGunActive() const
{
	AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
	return (GJCharacter && GJCharacter->CurrentGun == this);
}

void AGJMiniGun::EnableFire()
{
	bCanFire = true;
}

float AGJMiniGun::GetMaxGauge() const
{
	return MaxGauge;
}

float AGJMiniGun::GetCurrentGauge() const
{
	return CurrentGauge;
}

void AGJMiniGun::BeginPlay()
{
	Super::BeginPlay();

	/*FTimerHandle GaugeHandle;
	GetWorld()->GetTimerManager().SetTimer(
		GaugeHandle,
		[this]()
		{
			CurrentGauge = MaxGauge;
			UE_LOG(LogTemp, Warning, TEXT("Timer is over!"));
			if (CurrentGauge == MaxGauge)
			{
				ActivateMiniGun();
				UE_LOG(LogTemp, Warning, TEXT("MiniGun Activate!"));
			}
		},
		5.0f,
		false
	);*/
}

void AGJMiniGun::StartDeactivationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		MiniGunTimerHandle,
		this,
		&AGJMiniGun::DeactivateMiniGun,
		MiniGunDuration,
		false
	);

	// 매 프레임마다 CurrentGauge 감소 (0.1초마다 실행)
	GetWorld()->GetTimerManager().SetTimer(
		GaugeDecreaseTimerHandle,
		this,
		&AGJMiniGun::DecreaseGauge,
		0.1f,  // 0.1초마다 실행
		true
	);
}

void AGJMiniGun::ResetGauge()
{
	CurrentGauge = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("MiniGun Gauge Reset to 0!"));
}

void AGJMiniGun::DecreaseGauge()
{
	if (CurrentGauge > 0)
	{
		float DecreaseAmount = 100.0f / (MiniGunDuration / 0.1f); // MiniGunDuration 동안 100 -> 0으로 감소
		CurrentGauge = FMath::Clamp(CurrentGauge - DecreaseAmount, 0.0f, 100.0f);

		UE_LOG(LogTemp, Warning, TEXT("CurrentGauge: %f"), CurrentGauge);
	}
	else
	{
		// CurrentGauge가 0이 되면 감소 타이머 해제
		GetWorld()->GetTimerManager().ClearTimer(GaugeDecreaseTimerHandle);
	}
}
