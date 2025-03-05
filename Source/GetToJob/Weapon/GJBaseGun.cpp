#include "Weapon/GJBaseGun.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/GJCharacter.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/GJScope.h"
#include "Weapon/GJSilencer.h"
#include "Weapon/GJElementalRocketMagazine.h"
#include "Kismet/GameplayStatics.h"
#include "Character/GJInventoryComponent.h"



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
	CollisionComp->SetupAttachment(GunMesh); // 이거 RootComponent->GunMesh로 바꿈

	//// 플레이어가 총을 들고 있지 않을 때만 충돌 비활성화
	//GunMesh->SetSimulatePhysics(false);
	//GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GunMesh->SetCollisionObjectType(ECC_WorldDynamic);


	// 오버랩 이벤트를 바인딩 (일단 접촉 시 이벤트가 발생하는 것을 초기 설정으로)
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGJBaseGun::OnBeginOverlap);
	// 충돌 종료 이벤트 바인딩 (무기 범위를 벗어나면 호출됨)
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AGJBaseGun::OnEndOverlap);




	Damage = 100.0f;
	Accuracy = 100.0f;
	AmmoVelocity = 100.0f;
	FireRate = 200.0f;
	MaxAmmo = 90;
	MagazineCapacity = 30;
	CurrentAmmo = MagazineCapacity;


	bIsSilenced = false;
	bHasScope = false;
	ReloadTime = 3.0f;
	bIsReloading = false;
	bPickupGun = false;
	bCanPickup = true;
	bPickupMiniGun = false;
	MagazineCount = 100;
	MuzzleSocketName = TEXT("Muzzle");
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
			// 상호작용 가능한 무기 설정 (자동 줍기 제거)
			GJCharacter->InteractableWeapon = this;
			UE_LOG(LogTemp, Warning, TEXT("무기를 주울 준비 완료: %s"), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Pickup!!"));
		}
	}
}

void AGJBaseGun::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(OtherActor);
		if (GJCharacter && GJCharacter->InteractableWeapon == this)
		{
			// 플레이어가 더 이상 이 무기와 상호작용할 수 없음
			GJCharacter->InteractableWeapon = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("무기 상호작용 범위를 벗어남: %s"), *GetName());
		}
	}
}

void AGJBaseGun::EnablePickup()
{
	// 다시 충돌 활성화하여 주울 수 있도록 함
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	UE_LOG(LogTemp, Warning, TEXT("무기 줍기 가능"));
	bCanPickup = true;
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
	/*if (bIsReloading || MagazineCount <= 0 || MaxAmmo <= 0)
	{
		return;
	}*/
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsReloading!"));
		return;
	}
	if (MagazineCount <=0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MagazineCount!"));
		return;
	}
	if (MaxAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MaxAmmo!"));
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

	AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
	if (GJCharacter && GJCharacter->InventoryComponent)
	{
		if (GJCharacter->InventoryComponent->WeaponSlots.Contains(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup: 이미 총을 가지고 있음 - 줍기 불가"));
			return;
		}

		// 기존 무기 비활성화 (겹쳐 보이는 현상 방지)
		if (GJCharacter->CurrentGun)
		{
			GJCharacter->CurrentGun->SetActorHiddenInGame(true);
			GJCharacter->CurrentGun->SetActorEnableCollision(false);
		}

		// 인벤토리에 무기 추가
		GJCharacter->InventoryComponent->AddWeapon(this);

		// 이전 무기 상태 업데이트
		GJCharacter->PreviousWeaponType = GJCharacter->CurrentWeaponType;
	}

	// 자동 장착 로직
	if (!GJCharacter->CurrentGun)
	{
		GJCharacter->CurrentGun = GJCharacter->InventoryComponent->EquipWeaponFromSlot(0);
	}
	else
	{
		GJCharacter->EquipWeapon(this); // 자동 장착
	}

	// 물리 시뮬레이션 끄기
	GunMesh->SetSimulatePhysics(false);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// `SetSimulatePhysics(false);`가 확실히 적용되었는지 0.05초 후 확인
	FTimerHandle TimerHandle_CheckPhysics;
	GetWorldTimerManager().SetTimer(TimerHandle_CheckPhysics, [this]()
		{
			if (GunMesh->IsSimulatingPhysics())
			{
				UE_LOG(LogTemp, Error, TEXT("GunMesh 물리 시뮬레이션이 꺼지지 않음!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GunMesh 물리 시뮬레이션이 정상적으로 꺼짐!"));
			}
		}, 0.05f, false);

	// 0.05초 후 소켓에 부착 (물리 충돌 방지)
	FTimerHandle TimerHandle_Attach;
	GetWorldTimerManager().SetTimer(TimerHandle_Attach, [this, PlayerCharacter]()
		{
			if (!PlayerCharacter) return;

			UE_LOG(LogTemp, Warning, TEXT("GunSocketName 확인: %s"), *GunSocketName.ToString());

			// `AttachToComponent()` 실행 여부 확인
			bool bAttachSuccess = GunMesh->AttachToComponent(
				PlayerCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName
			);

			if (!bAttachSuccess)
			{
				UE_LOG(LogTemp, Error, TEXT("AttachToComponent 실패!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Pickup: 총이 정상적으로 소켓에 부착됨!"));

				// 부착 후 위치 갱신
				GunMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			}

		}, 0.05f, false);

	// 플레이어 소유 설정
	SetOwner(PlayerCharacter);
	bPickupGun = true;

	// 로그 출력 (최종 상태 업데이트)
	UE_LOG(LogTemp, Warning, TEXT("Pickup: 무기 장착 및 상태 업데이트 완료"));
}


void AGJBaseGun::ThrowAway()
{
	if (!bPickupGun)
	{
		UE_LOG(LogTemp, Error, TEXT("총 없음"));
		return;
	}

	// 소켓에서 분리
	GunMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	bCanPickup = false;

	// 캐릭터에서 현재 무기를 해제
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
	if (PlayerCharacter)
	{
		AGJCharacter* GJCharacter = Cast<AGJCharacter>(PlayerCharacter);
		if (GJCharacter && GJCharacter->CurrentGun == this)
		{
			GJCharacter->CurrentGun = nullptr;  // 확실히 해제
			UE_LOG(LogTemp, Warning, TEXT("ThrowAway: 총을 버려서 CurrentGun을 nullptr로 설정"));
		}
	}

	// 물리 활성화
	GunMesh->SetSimulatePhysics(true);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 줍기 감지 비활성화
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 바닥으로 자연스럽게 떨어지도록 힘 적용
	FVector ThrowDirection = PlayerCharacter ? PlayerCharacter->GetActorForwardVector() : FVector::ForwardVector;
	FVector ThrowForce = ThrowDirection * 200.0f + FVector(0.0f, 0.0f, 300.0f);
	GunMesh->AddImpulse(ThrowForce, NAME_None, true);

	// 0.5초 후 줍기 가능하게 설정
	FTimerHandle TimerHandle_EnablePickup;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnablePickup, this, &AGJBaseGun::EnablePickup, 0.5f, false);

	SetOwner(nullptr);
	bPickupGun = false;

	UE_LOG(LogTemp, Warning, TEXT("ThrowAway 완료 - 총이 바닥으로 떨어짐, 0.5초 후 다시 줍기 가능"));
}

void AGJBaseGun::FinishReload()
{
	int32 TempMax = MaxAmmo;
	MaxAmmo = FMath::Max(0, MaxAmmo - (MagazineCapacity - CurrentAmmo));
	UE_LOG(LogTemp, Warning, TEXT("Reload Finish!! MaxAmmo is %d | Using %d Ammo!!"), MaxAmmo, (MagazineCapacity - CurrentAmmo));
	CurrentAmmo = FMath::Min(MagazineCapacity, TempMax);
	bIsReloading = false;
}

void AGJBaseGun::EquipAttachment(AGJBaseGunAttachment* Attachment)
{
	if (!Attachment)
	{
		return;
	}

	// 부착물을 중복 장착 방지
	if (Attachments.Contains(Attachment))
	{
		UE_LOG(LogTemp, Warning, TEXT("This attachment is already equipped!"));
		return;
	}

	// 특정 부착물이 특정 무기만 장착 가능하도록 제한 
	if (AGJScope* GJScope = Cast<AGJScope>(Attachment))
	{
		if (GetGunType() != EGunType::Rifle) // Rifle이 아니면 장착 불가
		{
			UE_LOG(LogTemp, Warning, TEXT("EquipAttachment - Scopes can only be equipped on Rifles!"));
			return;
		}
	}

	// 특정 부착물이 특정 무기만 장착 가능하도록 제한 
	if (AGJSilencer* GJSilencer = Cast<AGJSilencer>(Attachment))
	{
		if (GetGunType() != EGunType::Revolver) // Rifle이 아니면 장착 불가
		{
			UE_LOG(LogTemp, Warning, TEXT("EquipAttachment - Silencer can only be equipped on Revolver!"));
			return;
		}
	}
	// 특정 부착물이 특정 무기만 장착 가능하도록 제한 
	if (AGJElementalRocketMagazine* GJElementalRocketMagazine = Cast<AGJElementalRocketMagazine>(Attachment))
	{
		if (GetGunType() != EGunType::RocketLauncher) // Rifle이 아니면 장착 불가
		{
			UE_LOG(LogTemp, Warning, TEXT("EquipAttachment - Scopes can only be equipped on RocketLauncher!"));
			return;
		}
	}


	// 부착물 부착
	Attachment->AttachToComponent(
		GunMesh,
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		Attachment->AttachmentSocketName
	);

	// 배열에 추가
	Attachments.Add(Attachment);
	Attachment->AttachToGun(this);

	// 장착한 부착물이 스코프인지 확인
	if (AGJScope* GJScope = Cast<AGJScope>(Attachment))
	{
		bHasScope = true;
	//	GJScope->EnableScopeView();
	}

	UE_LOG(LogTemp, Warning, TEXT("Attachment Equipped: %s"), *Attachment->GetName());
}

void AGJBaseGun::RemoveAttachment(AGJBaseGunAttachment* Attachment)
{
	if (!Attachment || !Attachments.Contains(Attachment))
	{
		return;
	}

	// 부착물 제거
	Attachments.Remove(Attachment);
	Attachment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Attachment->DetachFromGun();

	// 만약 제거한 부착물이 스코프였다면, 스코프 상태 업데이트
	if (AGJScope* GJScope = Cast<AGJScope>(Attachment))
	{
		bHasScope = false;
	//	GJScope->DisableScopeView();
	}

	UE_LOG(LogTemp, Warning, TEXT("Attachment Removed: %s"), *Attachment->GetName());
}

void AGJBaseGun::SwapAttachmentsWithGun(AGJBaseGun* OldWeapon, AGJBaseGun* NewWeapon)
{
	//if (!OldWeapon || !NewWeapon)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("SwapAttachments: Invalid weapon reference!"));
	//	return;
	//}

	//// 기존 무기의 부착물을 가져옴
	//TArray<AGJBaseGunAttachment*> OldAttachments = OldWeapon->Attachments;

	//// 기존 무기의 부착물 Detach & 숨기기
	//for (AGJBaseGunAttachment* Attachment : OldAttachments)
	//{
	//	if (Attachment)
	//	{
	//		RemoveAttachment(Attachment);
	//		Attachment->SetActorHiddenInGame(true); // 숨김
	//		Attachment->SetActorEnableCollision(false); // 충돌 비활성화
	//	}
	//}

	//

	//// 새로운 무기에 부착물 장착
	//for (AGJBaseGunAttachment* Attachment : OldAttachments)
	//{
	//	if (Attachment && NewWeapon)
	//	{
	//		// 새로운 무기에 부착
	//		Attachment->AttachToComponent(
	//			NewWeapon->GunMesh,
	//			FAttachmentTransformRules::SnapToTargetIncludingScale,
	//			Attachment->AttachmentSocketName
	//		);

	//		// 부착물 보이게 하기
	//		Attachment->SetActorHiddenInGame(false);
	//		Attachment->SetActorEnableCollision(true);

	//		// 새로운 무기의 부착물 리스트에 추가
	//		NewWeapon->Attachments.Add(Attachment);
	//		Attachment->AttachToGun(NewWeapon);
	//	}
	//}
	//// 기존 무기에서 부착물 리스트 초기화
	//OldWeapon->Attachments.Empty();

	//UE_LOG(LogTemp, Warning, TEXT("Attachments swapped from %s to %s"), *OldWeapon->GetName(), *NewWeapon->GetName());
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

int32 AGJBaseGun::GetMagazineCapacity() const
{
	return MagazineCapacity;
}

void AGJBaseGun::RaiseMaxAmmo(int32 InAmmo)
{
	MaxAmmo += InAmmo;
}

void AGJBaseGun::RaiseMagazineCapacity(int32 InMagazineCapacity)
{
	MagazineCapacity += InMagazineCapacity;
}

void AGJBaseGun::SetCurrentAmmo(int32 InAmmo)
{
	CurrentAmmo += InAmmo;
}

void AGJBaseGun::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

void AGJBaseGun::BeginPlay()
{
	Super::BeginPlay();

}

EGunType AGJBaseGun::GetGunType() const
{
	 return GunType; 
}

void AGJBaseGun::ApplyRecoil()
{
	AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (OwnerController)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
		if (PlayerController)
		{
			float RandomRecoil = FMath::RandRange(-0.1f, 0.1f);  // ✅ 좌우 랜덤 리코일 추가

			// 반동 적용
			PlayerController->AddPitchInput(-RecoilStrength);
			PlayerController->AddYawInput(RandomRecoil);

			// 반동 값 저장 (반동 복구를 위해)
			InitialRecoilStrength = -RecoilStrength;
			InitialYawRecoil = RandomRecoil;
			CurrentRecoilStrength = InitialRecoilStrength;
			CurrentYawRecoil = InitialYawRecoil;


			// 일정 시간 후 반동 회복 시작
			GetWorldTimerManager().ClearTimer(RecoilResetTimer);
			GetWorldTimerManager().SetTimer(
				RecoilResetTimer,
				this,
				&AGJBaseGun::ResetRecoil,
				0.05f,  // 반동 회복 주기
				true
			);
		}
	}
}

void AGJBaseGun::ResetRecoil()
{
	AController* OwnerController = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (OwnerController)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
		if (PlayerController)
		{
			float RecoveryRate = 0.05f; // 반동 회복 속도 (작을수록 부드러움)

			if (FMath::Abs(CurrentRecoilStrength) > 0.01f || FMath::Abs(CurrentYawRecoil) > 0.01f)
			{
				PlayerController->AddPitchInput(-CurrentRecoilStrength * RecoveryRate);
				PlayerController->AddYawInput(CurrentYawRecoil * RecoveryRate);

				// 반동 수치 점진적으로 줄이기
				CurrentRecoilStrength *= (1.0f - RecoveryRate);
				CurrentYawRecoil *= (1.0f - RecoveryRate);
			}
			else
			{
				// 반동이 거의 원래대로 돌아오면 타이머 중지
				GetWorldTimerManager().ClearTimer(RecoilResetTimer);
				UE_LOG(LogTemp, Warning, TEXT("Recoil Finish!!"));
				CurrentRecoilStrength = 0.0f;
				CurrentYawRecoil = 0.0f;
			}
		}
	}
}

