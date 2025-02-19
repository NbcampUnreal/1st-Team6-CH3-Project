// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJRocketProjectile.h"
#include "GJRocketProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // ProjectileMovement�� ���� �ʿ�
#include "Kismet/GameplayStatics.h"

// Sets default values
AGJRocketProjectile::AGJRocketProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// �浹 ������ ���� �ݸ����� �߰�
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionComp;

	// ���Ͽ� �޽� �߰�
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);// ���� �޽� ��ü�� �浹���� �ʵ���

	// ���� źȯ�� �̵� ������Ʈ �߰�
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bAllowAnyoneToDestroyMe = false;
	ProjectileMovement->bRotationFollowsVelocity = true; // źȯ�� �̵��ϴ� ���⿡ ���� �ڵ����� ȸ��
	// -> �̵��ϸ鼭 ������ �����ϴ� ��ü�� ����
	
	// �浹 �̺�Ʈ�� ���ε�
	CollisionComp->OnComponentHit.AddDynamic(this, &AGJRocketProjectile::OnImpact);

	// ������ ����, �ڵ� ���� ����
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
	// ���� ȿ�� ���
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	// ���� �������� ����
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

	// ������ ����
	Destroy();
}

void AGJRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ���� �ð� �� �ڵ����� ����
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AGJRocketProjectile::AutoExplode,
		RocketLifetime,
		false
	);
}





