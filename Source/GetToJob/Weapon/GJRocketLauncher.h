// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGun.h"
#include "GJRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class GETTOJOB_API AGJRocketLauncher : public AGJBaseGun
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float ExplosionRadius = 300.f;
};
