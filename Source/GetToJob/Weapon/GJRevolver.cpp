#include "Weapon/GJRevolver.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void AGJRevolver::Fire()
{
	// ������ ���̰ų�, ź�� ���� ��� �߻� �Ұ�
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		return;
	}
	
	// ź�� �Һ�
	CurrentAmmo--;

	//ĳ������ ��Ʈ�ѷ����� ���� ������ �������� �Լ� (SetOwner�� �̿��ؼ� ĳ���Ϳ� ���� ���� ���Ѿ� �Ѵ�.)
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
}

void AGJRevolver::EnableFire()
{
	bCanFire = true;
}
