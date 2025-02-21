// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJRocketProjectile.h"
#include "GJRocketProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // ProjectileMovement를 위해 필요
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"


// Sets default values
AGJRocketProjectile::AGJRocketProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 발사자의 충돌을 무시하도록 설정
	if (GetInstigator())
	{
		CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
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
	if (ExplosionEffect)
	{
		UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

		if (SpawnedEffect)
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
				3.0f,
				false
			);
		}
	}
	// 폭발 반경 내의 액터들을 배열에 저장
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 캐릭터만 감지
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody)); // 물리 오브젝트도 포함

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this); // 자기 자신 제외

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
		OverlappedActors,
		this,
		GetInstigatorController(),
		true
	);


	for (AActor* Actor : OverlappedActors)
	{
		if (Actor && Actor->ActorHasTag("NPC")) // NPC 태그를 확인
		{
			FVector ImpactDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ApplyKnockback(Actor, -ImpactDirection);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("This Actor is not NPC! -> %s"), *Actor->GetName());
		}
	}

	// 로켓을 삭제
	Destroy();
}

void AGJRocketProjectile::ApplyKnockback(AActor* HitActor, FVector ImpactNormal)
{
	UE_LOG(LogTemp, Warning, TEXT("This Actor is not CharacterClass!"))
	// 액터가 ACharacter인 경우 (NPC가 캐릭터를 상속받을 경우)
	ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
	if (HitCharacter)
	{
		FVector KnockbackDirection = -ImpactNormal * 1000.0f; // 충돌 방향의 반대로 넉백
		KnockbackDirection.Z = 300.0f; //위쪽으로 살짝 띄우기

		HitCharacter->LaunchCharacter(KnockbackDirection, true, true);
		return;
	}
}

void AGJRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	

	// 일정 시간 후 자동으로 폭발
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AGJRocketProjectile::AutoExplode,
		RocketLifetime,
		false
	);
}





