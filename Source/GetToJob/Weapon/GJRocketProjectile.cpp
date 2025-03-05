// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJRocketProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // ProjectileMovement를 위해 필요
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Character/GJCharacter.h"
#include "NPC/GJNPC.h"
#include "Weapon/GJMiniGun.h"
#include "Weapon/EGJElementalType.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGJRocketProjectile::AGJRocketProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	

	// 발사자의 충돌을 무시하도록 설정
	AGJCharacter* InstigatorCharacter = Cast<AGJCharacter>(GetInstigator());
	if (InstigatorCharacter)
	{
		CollisionComp->IgnoreActorWhenMoving(InstigatorCharacter, true);
	}

	if (GetOwner())
	{
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}

	// 충돌 감지를 위한 콜리전을 추가
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(10.0f);
	RootComponent = CollisionComp;

	// 로켓에 메시 추가, 콜리전 설정
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	
	
	// 물리 탄환의 이동 컴포넌트 추가
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bAllowAnyoneToDestroyMe = false;
	ProjectileMovement->bRotationFollowsVelocity = true; // 탄환이 이동하는 방향에 따라 자동으로 회전
	// -> 이동하면서 방향을 유지하는 물체에 적합
	
	// 충돌 이벤트를 바인딩
	CollisionComp->OnComponentHit.AddDynamic(this, &AGJRocketProjectile::OnImpact);

	// 데미지 설정, 자동 폭발 설정
	DamageRadius = 300.0f;
	DamageAmount = 50.0f;
	RocketLifetime = 3.0f;

	ShockDelay = 2.0f;
	FreezeDelay = 4.0f;
	BurnDelay = 5;
	
	ExplosionEffect = nullptr;
	ShockEffect = nullptr;
	FreezeEffect = nullptr;
	BurnEffect = nullptr;
}

void AGJRocketProjectile::OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rocket Impact with : %s"), *OtherActor->GetName());

		// NPC 태그가 있는지 확인
		if (OtherActor->ActorHasTag("NPC"))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is an NPC! Applying knockback."), *OtherActor->GetName());

			// 넉백 적용 함수 호출
			ApplyKnockback(OtherActor, Hit.ImpactNormal);
		}

		else if(OtherActor->ActorHasTag("Ground"))
		{

			UE_LOG(LogTemp, Warning, TEXT("Ground!"));
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("This Actor is not CharacterClass!"))
		}
	}

	if (OtherActor && OtherActor != GetInstigator())
	{
		AutoExplode();
	}
}

void AGJRocketProjectile::AutoExplode()
{
	// 사운드 재생
	float AdjustVolume = 1.0f;
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		FireSound,
		GetActorLocation(),
		AdjustVolume
	);


	// 속성별 폭발 파티클 생성
	SpawnElementalParticle();
	//if (ExplosionEffect)
	//{
	//	UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	//	if (SpawnedEffect)
	//	{
	//		// 3초 후에 이펙트를 제거하는 타이머 설정
	//		FTimerHandle ExplosionEffectTimer;
	//		GetWorldTimerManager().SetTimer(
	//			ExplosionEffectTimer,
	//			[SpawnedEffect]()
	//			{
	//				if (SpawnedEffect)
	//				{
	//					SpawnedEffect->DeactivateSystem(); // 이펙트 중지
	//					SpawnedEffect->DestroyComponent(); // 컴포넌트 삭제
	//				}
	//			},
	//			3.0f,
	//			false
	//		);
	//	}
	//}
	// 폭발 반경 내의 액터들을 배열에 저장
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 캐릭터만 감지
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody)); // 물리 오브젝트도 포함
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Knockback 오브젝트 포함

	TArray<AActor*> IgnoreActors;
	//	IgnoreActors.Add(this); // 자기 자신 제외

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		DamageRadius,
		ObjectTypes,
		nullptr,
		IgnoreActors,
		OverlappedActors
	);

	// 폭발 데미지를 적용
	UGameplayStatics::ApplyRadialDamage(
		this,
		DamageAmount,
		GetActorLocation(),
		DamageRadius,
		nullptr,
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
	);


	for (AActor* Actor : OverlappedActors)
	{
		if (Actor && Actor->ActorHasTag("NPC")) // NPC 태그를 확인
		{
			if (APawn* HitPawn = Cast<APawn>(Actor))
			{
				if (ACharacter* HitCharacter = Cast<ACharacter>(HitPawn))
				{
					if (UCharacterMovementComponent* MovementComp = HitCharacter->GetCharacterMovement())
					{
						float OriginalSpeed = MovementComp->MaxWalkSpeed;
						// 속성 적용
						switch (ElementalType)
						{
						case EGJElementalType::Shock:
							// 이동 정지
							MovementComp->DisableMovement();
							UE_LOG(LogTemp, Warning, TEXT("Shock Effect Applied: %s is stunned for 2 seconds"), *HitCharacter->GetName());
							// ShockDelay 후 이동 가능
							GetWorld()->GetTimerManager().SetTimer(
								EffectTimerHandle,
								[MovementComp]()
								{
									if (MovementComp)
									{
										MovementComp->SetMovementMode(MOVE_NavWalking);
										UE_LOG(LogTemp, Warning, TEXT("Shock Effect Ended: Character can move again"));
									}
								},
								2.0f,
								false
							);
							break;

						case EGJElementalType::Freeze:
							
							MovementComp->MaxWalkSpeed *= 0.5f;
							UE_LOG(LogTemp, Warning, TEXT("Freeze Effect Applied: %s speed reduced to %f"), *HitCharacter->GetName(), MovementComp->MaxWalkSpeed);

							GetWorld()->GetTimerManager().SetTimer(
								EffectTimerHandle,
								[MovementComp, OriginalSpeed]()
								{
									if (MovementComp)
									{
										MovementComp->MaxWalkSpeed = OriginalSpeed;
										UE_LOG(LogTemp, Warning, TEXT("Freeze Effect Ended: Speed restored to %f"), OriginalSpeed);
									}
								},
								FreezeDelay,
								false
							);
							break;


						case EGJElementalType::Burn:
							for (int i = 1; i < BurnDelay; i++)
							{
								GetWorld()->GetTimerManager().SetTimer(
									EffectTimerHandle,
									[HitCharacter]()
									{
										if (HitCharacter)
										{
											HitCharacter->TakeDamage(50.0f, FDamageEvent(), nullptr, nullptr);
											UE_LOG(LogTemp, Warning, TEXT("Burn Damage Applied: %s took 50 damage"), *HitCharacter->GetName());
										}
									},
									i,
									false
								);
							}
							break;
						default:
							break;
						}
					}
				}
			}
			

			FVector ImpactDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ApplyKnockback(Actor, -ImpactDirection);
			AGJNPC* HitEnemy = Cast<AGJNPC>(Actor);
			if (HitEnemy && HitEnemy->GetNPCDead())
			{
				AGJCharacter* GJCharacter = Cast<AGJCharacter>(GetInstigator());
				UE_LOG(LogTemp, Warning, TEXT("NPC is Dead! Increasing MiniGun Gauge!"));
				if (GJCharacter && GJCharacter->MiniGun)
				{
					GJCharacter->MiniGun->IncreaseGauge(7.0f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Character or No Minigun"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("This Actor is not NPC! -> %s"), *Actor->GetName());
		}
		
		
		// OverlappedActors가 무엇인지 출력 디버그용
		if (Actor)
		{
			FString ActorTags;
			for (const FName& Tag : Actor->Tags)
			{
				ActorTags += Tag.ToString() + TEXT(" ");
			}

			UE_LOG(LogTemp, Warning, TEXT("Actor: %s | Class: %s"),
				*Actor->GetName(),
				*Actor->GetClass()->GetName()
			);

		}

		// 로켓을 삭제
		Destroy();
	}
}
void AGJRocketProjectile::ApplyKnockback(AActor* HitActor, FVector ImpactNormal)
{
	UE_LOG(LogTemp, Warning, TEXT("This Actor is not CharacterClass!"));

	// 액터가 ACharacter인 경우 (NPC가 캐릭터를 상속받을 경우)
	ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
	if (HitCharacter)
	{
		FVector KnockbackDirection = -ImpactNormal * 2000.0f; // 충돌 방향의 반대로 넉백
		KnockbackDirection.Z = 300.0f; //위쪽으로 살짝 띄우기

		HitCharacter->LaunchCharacter(KnockbackDirection, true, true);
		return;
	}
}

void AGJRocketProjectile::SetMaterial()
{
	// 로켓 머터리얼 설정
	UMaterialInterface* NewMaterial = nullptr;
	switch (ElementalType)
	{
	case EGJElementalType::Shock:
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/VREditor/LaserPointer/TranslucentHoverMaterial.TranslucentHoverMaterial"));
		UE_LOG(LogTemp, Warning, TEXT("Yellow Material"));
		break;
	case EGJElementalType::Freeze:
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/VREditor/UI/KeyDiskMaterial.KeyDiskMaterial"));
		UE_LOG(LogTemp, Warning, TEXT("Blue Material"));
		break;
	case EGJElementalType::Burn:
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/VREditor/LaserPointer/TranslucentLaserPointerMaterialInst.TranslucentLaserPointerMaterialInst"));
		UE_LOG(LogTemp, Warning, TEXT("Red Material"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No Material"));
		break;
	}
	// 로켓 머터리얼 적용
	if (NewMaterial)
	{
		RocketMesh->SetMaterial(0, NewMaterial);
	}
}

void AGJRocketProjectile::SpawnElementalParticle()
{
	UParticleSystemComponent* SpawnedEffect = nullptr;
	FTimerHandle ExplosionEffectTimer;
	switch (ElementalType)
	{
	case EGJElementalType::Shock:
		if (ShockEffect)
		{
			SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShockEffect, GetActorLocation());
			SpawnedEffect->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

			if (SpawnedEffect)
			{
				// 3초 후에 이펙트를 제거하는 타이머 설정

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
					2.0f,
					false
				);
			}
			UE_LOG(LogTemp, Warning, TEXT("Spawn is Shock"));
		}
		break;
	case EGJElementalType::Freeze:
		if (FreezeEffect)
		{
			SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FreezeEffect, GetActorLocation());
			SpawnedEffect->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

			if (SpawnedEffect)
			{
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
					4.0f,
					false
				);
			}
			UE_LOG(LogTemp, Warning, TEXT("Freeze is Shock"));
		}
		break;
	case EGJElementalType::Burn:
		if (BurnEffect)
		{
			SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BurnEffect, GetActorLocation());

			if (SpawnedEffect)
			{
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
					3.0f,
					false
				);
			}
			UE_LOG(LogTemp, Warning, TEXT("Spawn is Burn"));
		}
		break;
	default:
		if (ExplosionEffect)
		{
			SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

			if (SpawnedEffect)
			{
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
					3.0f,
					false
				);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Spawn is Explosion"));

	}
	
}

void AGJRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MaterialTimer;
	GetWorld()->GetTimerManager().SetTimer(
		MaterialTimer,
		this,
		&AGJRocketProjectile::SetMaterial,
		0.1f,
		false
	);
	

	// 일정 시간 후 자동으로 폭발
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AGJRocketProjectile::AutoExplode,
		RocketLifetime,
		false
	);

}





