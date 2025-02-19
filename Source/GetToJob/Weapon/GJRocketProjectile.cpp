// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJRocketProjectile.h"
#include "GJRocketProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // ProjectileMovement를 위해 필요
#include "Kismet/GameplayStatics.h"

// Sets default values
AGJRocketProjectile::AGJRocketProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 충돌 감지를 위한 콜리전을 추가
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionComp;

	// 로켓에 메시 추가
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);// 로켓 메시 자체는 충돌하지 않도록

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
	if (OtherActor && OtherActor != GetInstigator())
	{
		AutoExplode();
	}
}

void AGJRocketProjectile::AutoExplode()
{
	// 폭발 효과 재생
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

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

	// 로켓을 삭제
	Destroy();
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





