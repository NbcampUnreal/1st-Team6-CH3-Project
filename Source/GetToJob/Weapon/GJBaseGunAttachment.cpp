// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GJBaseGunAttachment.h"

// Sets default values
AGJBaseGunAttachment::AGJBaseGunAttachment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGJBaseGunAttachment::AttachToGun(ABaseGun* Gun)
{
}

void AGJBaseGunAttachment::DetachToGun(ABaseGun* Gun)
{
}

// Called when the game starts or when spawned
void AGJBaseGunAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGJBaseGunAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

