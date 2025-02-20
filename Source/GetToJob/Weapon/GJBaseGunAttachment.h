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
	AGJBaseGunAttachment();

	UFUNCTION()
	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	UFUNCTION()
	virtual void DetachFromGun() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
	USceneComponent* RootComp;
	UPROPERTY()
	AGJBaseGun* AttachedGun;



	virtual void BeginPlay() override;

public:	


};
