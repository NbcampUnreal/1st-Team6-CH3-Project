#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "GJSilencer.generated.h"

UCLASS()
class GETTOJOB_API AGJSilencer : public AGJBaseGunAttachment
{
	GENERATED_BODY()
	
public:
	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	virtual void DetachFromGun() override;
	
};
