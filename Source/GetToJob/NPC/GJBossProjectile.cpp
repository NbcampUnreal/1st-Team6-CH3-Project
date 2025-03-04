#include "NPC/GJBossProjectile.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AGJBossProjectile::AGJBossProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(SceneRoot);

	if (GetInstigator())
	{
		HitArea->IgnoreActorWhenMoving(GetInstigator(), true);
	}

	HitArea = CreateDefaultSubobject<USphereComponent>(TEXT("HitArea"));
	HitArea->SetupAttachment(SceneRoot);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComponent->InitialSpeed = 800;
	ProjectileComponent->MaxSpeed = 8500;
	ProjectileComponent->bAllowAnyoneToDestroyMe = false;
	ProjectileComponent->bRotationFollowsVelocity = true;

	Damage = 10.f;
	DamageRadius = 200.f;
	ProjectileLifetime = 4.f;

}

void AGJBossProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Overlap: OtherActor=%s, Instigator=%s"), *GetNameSafe(OtherActor), *GetNameSafe(GetInstigator()));

	if (OtherActor && (OtherActor == GetInstigator() || OtherActor == this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Overlap: Instigator collision ignored."));
		return; // Instigator와 충돌 시 무시
	}

	if (OtherActor && OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Overlap: AutoExplode triggered."));
		AutoExplode();
	}
}

void AGJBossProjectile::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AGJBossProjectile::AutoExplode()
{
	if (Particle)
	{
		UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation());
		if (SpawnedEffect)
		{
			FTimerHandle ExplosionEffectTimer;
			GetWorldTimerManager().SetTimer(
				ExplosionEffectTimer,
				[SpawnedEffect]()
				{
					if (SpawnedEffect)
					{
						SpawnedEffect->DeactivateSystem();
						SpawnedEffect->DestroyComponent();
					}
				},
				3.0f,
				false
			);
		}
	}
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

	TArray<AActor*> IgnoreActors;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		DamageRadius,
		ObjectTypes,
		nullptr,
		IgnoreActors,
		OverlappedActors
	);
	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GetActorLocation(),
		DamageRadius,
		nullptr,
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true
	);
	Destroy();
}

void AGJBossProjectile::BeginPlay()
{
	Super::BeginPlay();

	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AGJBossProjectile::BeginOverlap);
	HitArea->OnComponentEndOverlap.AddDynamic(this, &AGJBossProjectile::EndOverlap);
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AGJBossProjectile::AutoExplode,
		ProjectileLifetime,
		false
	);
}

