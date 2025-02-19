// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/GJGunAttachmentInterface.h"
#include "GJBaseGunAttachment.generated.h"

UCLASS()
class GETTOJOB_API AGJBaseGunAttachment : public AActor, public IGJGunAttachmentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGJBaseGunAttachment();

	virtual void AttachToGun(class ABaseGun* Gun) override;
	virtual void DetachToGun(class ABaseGun* Gun) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
