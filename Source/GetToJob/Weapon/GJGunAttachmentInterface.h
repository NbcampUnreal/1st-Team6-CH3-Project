#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h" 
#include "GJGunAttachmentInterface.generated.h"


UINTERFACE(MinimalAPI)
class UGJGunAttachmentInterface : public UInterface
{
	GENERATED_BODY()
};

class GETTOJOB_API IGJGunAttachmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachToGun(class AGJBaseGun* Gun) = 0;
	virtual void DetachFromGun() = 0;
};
