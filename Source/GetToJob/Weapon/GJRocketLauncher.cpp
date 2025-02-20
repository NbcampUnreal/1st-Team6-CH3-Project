#include "Weapon/GJRocketLauncher.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/GJRocketProjectile.h"



AGJRocketLauncher::AGJRocketLauncher()
{
	FireSound = nullptr;
	FireRate = 20.0f;
	CoolDownDelay = 1 / (FireRate / 60);
	InitialSpeed = 20000.0f;
	bCanFire = true;
	bIsReloading = false;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 4.0f;
	bPickRocketLauncher= false;
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

			// 캐릭터가 가진 소켓 위치 가져오기 // TODO 나중에 총구 소켓(Muzzle)을 만들어서 변경
			AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
			FVector MuzzleLocation = FVector::ZeroVector;

			if (GJCharacter && GJCharacter->GetMesh())
			{
				MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("Rocket"));
			}
			else
			{
				// 못 가져올 경우에는 카메라 Location을 사용
				MuzzleLocation = CameraLocation;
			}

			// 방향 벡터 계산
			FVector LaunchDirection = (CameraLocation - MuzzleLocation).GetSafeNormal(); // 방향 벡터

			// 속도
			InitialSpeed = 20000.0f;

			// 로켓 생성
			AGJRocketProjectile* SpawnedRocket = GetWorld()->SpawnActor<AGJRocketProjectile>(
				ProjectileClass,
				MuzzleLocation,
				CameraRotation
			);

			if (SpawnedRocket)
			{
				// 로켓을 발사한 폰 설정
				SpawnedRocket->SetInstigator(OwnerPawn);
				// 로켓의 속도 초기화
				UProjectileMovementComponent* ProjectileMovement = SpawnedRocket->FindComponentByClass<UProjectileMovementComponent>();
				if (ProjectileMovement)
				{
					ProjectileMovement->Velocity = CameraRotation.Vector() * InitialSpeed; // 방향 & 속도 설정
					ProjectileMovement->bRotationFollowsVelocity = true; // 이동 방향 따라 회전
					ProjectileMovement->Activate(); // 이동 활성화

					UE_LOG(LogTemp, Warning, TEXT("Final Velocity: X=%f, Y=%f, Z=%f"),
						ProjectileMovement->Velocity.X,
						ProjectileMovement->Velocity.Y,
						ProjectileMovement->Velocity.Z);
				}
			}
		}
	}
	// 사격 속도에 따른 Delay 필요
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
		&AGJRocketLauncher::FinishReload,
		ReloadTime,
		false
	);
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

void AGJRocketLauncher::Pickup(ACharacter* PlayerCharacter)
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
		TEXT("Rocket")
	);

	// 플레이어가 총을 소유
	SetOwner(PlayerCharacter);
	bPickRocketLauncher = false;

	// 캐릭터가 가진 현재 총 = 장착한 총
	if (GJCharacter)
	{
		GJCharacter->CurrentGun = this;
	}

	// 주운 이후에는 콜리전 제거
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

