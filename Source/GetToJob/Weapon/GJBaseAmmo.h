#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GJBaseGun.h"
#include "GJBaseAmmo.generated.h"

UCLASS()
class GETTOJOB_API AGJBaseAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGJBaseAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	EGunType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionComp;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};
