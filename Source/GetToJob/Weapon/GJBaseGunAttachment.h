#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/GJGunAttachmentInterface.h"
#include "GJBaseGunAttachment.generated.h"

class USphereComponent;

UCLASS()
class GETTOJOB_API AGJBaseGunAttachment : public AActor, public IGJGunAttachmentInterface
{
	GENERATED_BODY()
	
public:	
	AGJBaseGunAttachment();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
	USceneComponent* RootComp;
	UPROPERTY()
	USphereComponent* CollisionComp;


	UFUNCTION()
	virtual void AttachToGun(class AGJBaseGun* Gun) override;
	UFUNCTION()
	virtual void DetachFromGun() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlapeedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:

	UPROPERTY()
	AGJBaseGun* AttachedGun;



	virtual void BeginPlay() override;

public:	


};
