#pragma once

#include "CoreMinimal.h"
#include "Weapon/GJBaseGunAttachment.h"
#include "GJScope.generated.h"

UCLASS()
class GETTOJOB_API AGJScope : public AGJBaseGunAttachment
{
	GENERATED_BODY()
	
public:
	AGJScope();

	// Scope의 기능 조준을 구현
	UFUNCTION()
	void EnableScopeView();
	UFUNCTION()
	void DisableScopeView();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scope")
	float ZoomFOV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scope")
	float DefaultFOV;
	UPROPERTY()
	APlayerController* PlayerController;



	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	virtual void DetachFromGun() override;

private:
	
};
