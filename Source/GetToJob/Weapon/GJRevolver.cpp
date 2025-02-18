#include "Weapon/GJRevolver.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void AGJRevolver::Fire()
{
	// 재장전 중이거나, 탄이 없을 경우 발사 불가
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}
	
	// 탄을 소비
	CurrentAmmo--;

	//캐릭터의 컨트롤러에서 시점 정보를 가져오는 함수 (SetOwner를 이용해서 캐릭터에 총을 장착 시켜야 한다.)
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
			// 디버그용 선 그리기(나중에 제거)
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 1.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.f);
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
}

void AGJRevolver::EnableFire()
{
	bCanFire = true;
}
