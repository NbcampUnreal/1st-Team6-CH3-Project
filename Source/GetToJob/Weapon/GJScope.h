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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scope")
	float CameraRelativeX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scope")
	float CameraRelativeY;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scope")
	float CameraRelativeZ;
	UPROPERTY()
	APlayerController* PlayerController;
	



	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	virtual void DetachFromGun() override;

	void FixZoomInRotation();
	void FixZoomOutRotation();

private:

	// 줌인 관련 함수
	FVector StoredCameraRelativeLocation;
	FRotator StoredCameraRotation;
	FRotator StoredControlRotation;
	FRotator StoredSpringArmRotation;
	float StoredSpringArmLength;

	FTimerHandle ZoomDelayTimer;
	
};
