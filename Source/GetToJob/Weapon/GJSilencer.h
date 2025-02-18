// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "GJSilencer.generated.h"

/**
 * 
 */
UCLASS()
class GETTOJOB_API AGJSilencer : public AGJBaseGunAttachment
{
	GENERATED_BODY()
	
public:
	virtual void AttachToGun(class ABaseGun* Gun) override;
	virtual void DetachToGun(class ABaseGun* Gun) override;
	
};
