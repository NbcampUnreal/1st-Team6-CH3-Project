#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "Weapon/EGJElementalType.h"
#include "GJElementalRocketMagazine.generated.h"

UCLASS()
class GETTOJOB_API AGJElementalRocketMagazine : public AGJBaseGunAttachment
{
	GENERATED_BODY()
	
public:
	AGJElementalRocketMagazine();


	UPROPERTY(EditAnywhere, Category = "Magazine")
	TArray<EGJElementalType> ElementalSequence; // 탄창 속성 순서

	int32 ShotCount; // 몇 번째 탄인지 카운트


	virtual void AttachToGun(AGJBaseGun* Gun) override;
	virtual void DetachFromGun() override;

	UPROPERTY()
	class AGJRocketLauncher* AttachedLauncher;

protected:
	UPROPERTY(EditAnywhere, Category = "Magazine")
	int32 ExtraAmmoCapacity; // 추가 탄약
	

	
};
