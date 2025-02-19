// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GJGunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGJGunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GETTOJOB_API IGJGunInterface
{
	GENERATED_BODY()

public:

	virtual void Fire() = 0;
	virtual void Reload() = 0;
	virtual bool IsReloading() = 0;
	virtual float GetDamage() = 0;
	virtual void Pickup(ACharacter* PlayerCharacter) = 0;
	virtual int32 GetCurrentAmmo() const = 0;
	virtual int32 GetMaxAmmo() const = 0;
};
