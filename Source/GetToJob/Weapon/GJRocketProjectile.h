// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GJRocketProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class GETTOJOB_API AGJRocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGJRocketProjectile();

	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionComp;
	// 메시 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RocketMesh;
	//충돌 시 폭발하는 효과
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Effect")
	UParticleSystem* ExplosionEffect;
	// 폭발 데미지와 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Damage")
	float DamageRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Damage")
	float DamageAmount;
	// 로켓 지속 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Lifetime")
	float RocketLifetime;
	// 로켓 지속 타이머
	FTimerHandle DestroyTimerHandle;

	// 충돌 함수: 충돌 시 폭발
	UFUNCTION()
	void OnImpact(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	// 일정 시간 후 폭발
	void AutoExplode();

	void ApplyKnockback(AActor* HitActor, FVector ImpactNormal);

protected:
	virtual void BeginPlay() override;
};
