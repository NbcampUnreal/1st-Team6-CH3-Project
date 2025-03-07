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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComp")
	USphereComponent* CollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMeshComp")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerComp")
	USphereComponent* TriggerComp;


	// Attachment를 부착할 소켓의 이름
	UPROPERTY(EditDefaultsOnly, Category = "Attachment")
	FName AttachmentSocketName = TEXT("AttachmentSocket");



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

	UFUNCTION()
	void OnTriggerOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

protected:

	UPROPERTY()
	AGJBaseGun* AttachedGun;



	virtual void BeginPlay() override;

private:
	bool bPickupAttachment;

};
