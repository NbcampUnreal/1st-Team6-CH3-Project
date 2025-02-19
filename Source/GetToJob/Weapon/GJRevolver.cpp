#include "Weapon/GJRevolver.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "Character/GJCharacter.h"
#include "Kismet/GameplayStatics.h"

AGJRevolver::AGJRevolver()
{
	PrimaryActorTick.bCanEverTick = false;


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


void AGJRevolver::Fire()
{
	
	// ������ ���̰ų�, ź�� ���� ��� �߻� �Ұ�
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Fire!!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	// ź�� �Һ�
	CurrentAmmo--;

	// �� �Ҹ� ���
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			FireSound,
			GetActorLocation()
		);
	}
	//ĳ������ ��Ʈ�ѷ����� ���� ������ �������� �Լ� 
	AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (OwnerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// ĳ���Ͱ� ���� ���� ��ġ �������� // TODO ���߿� �ѱ� ����(Muzzle)�� ���� ����
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetOwner());
		FVector MuzzleLocation = FVector::ZeroVector;

		if (GJCharacter && GJCharacter->GetMesh())
		{
			MuzzleLocation = GJCharacter->GetMesh()->GetSocketLocation(TEXT("Revolver"));
		}
		else
		{
			// �� ������ ��쿡�� ī�޶� Location�� ���
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
			// ������ �� ����� ����
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		}
		else
		{
			// �� ������ �� ����� ����
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f);
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

void AGJRevolver::Reload()
{
	// �������� �� �ʿ䰡 ���� ��
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

	// �ִ� ��Ÿ�� ���� (TODO: add Anim Montage)
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
