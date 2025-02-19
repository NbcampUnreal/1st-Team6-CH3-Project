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

	USphereComponent* CollisionComp;
	

	UProjectileMovementComponent* ProjectileMovement;

	// �޽� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RocketMesh;
	//�浹 �� �����ϴ� ȿ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Effect")
	UParticleSystem* ExplosionEffect;
	// ���� �������� �ݰ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Damage")
	float DamageRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Damage")
	float DamageAmount;
	// ���� ���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion | Lifetime")
	float RocketLifetime;
	// ���� ���� Ÿ�̸�
	FTimerHandle DestroyTimerHandle;

	// �浹 �Լ�: �浹 �� ����
	UFUNCTION()
	void OnImpact(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);



	// ���� �ð� �� ����
	void AutoExplode();

protected:
	virtual void BeginPlay() override;
};
