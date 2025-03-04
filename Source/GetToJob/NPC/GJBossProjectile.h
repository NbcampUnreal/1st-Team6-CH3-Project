#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GJBossProjectile.generated.h"

UCLASS()
class GETTOJOB_API AGJBossProjectile : public AActor
{
	GENERATED_BODY()

public:
	AGJBossProjectile();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 충돌이 종료될시에 호출되는 델리게이트에 등록하는 함수
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, Category = "Projectile | Status")
	float Damage;


	UPROPERTY(EditAnywhere, Category = "Projectile | Status")
	float DamageRadius;


	UPROPERTY(EditAnywhere, Category = "Projectile | Status")
	float ProjectileLifetime;

	FTimerHandle DestroyTimerHandle;

	void AutoExplode();
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileComponent;
protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
	USphereComponent* HitArea;
};
