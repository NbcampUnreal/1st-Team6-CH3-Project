#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "GJSilencer.generated.h"

UCLASS()
class GETTOJOB_API AGJSilencer : public AGJBaseGunAttachment
{
	GENERATED_BODY()
	
public:
	AGJSilencer();

	//소음 감소 비율 (0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, Category = "Silencer")
	float NoiseReductionMultilplier = 0.5f;
	// 반동 감소 비율 (0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, Category = "Silencer")
	float RecoilReductionMultiplier = 0.7f;
	// 데미지 증가 비율
	UPROPERTY(EditAnywhere, Category = "Silencer")
	float DamageMultiplier = 1.1f;
	
	
	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	virtual void DetachFromGun() override;
	
};
