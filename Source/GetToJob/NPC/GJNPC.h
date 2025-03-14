#pragma once

#include "CoreMinimal.h"
#include "NPC/AICharacterBase.h"
#include "NPC/CombatInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PatrolPath.h"
#include "Animation/AnimMontage.h"
#include "GJNPC.generated.h"

UCLASS()
class GETTOJOB_API AGJNPC : public AAICharacterBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	AGJNPC();

	UBehaviorTree* GetBehaviorTree() const; //Tree Return �Լ�

	APatrolPath* GetPatrolParth() const;

	UAnimMontage* GetMontage() const;

	void SetPatrolPath(APatrolPath* Path);

	void SetBehaviorTree(UBehaviorTree* ChooseTree);

	bool GetNPCDead() const;

	void SetNPCDead(bool IsDead);

	int MeleeAttack_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnDropItem();

protected:
	virtual void BeginPlay() override;
	
	bool IsNPCDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HeadCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	TSubclassOf<AActor> FirstAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	TSubclassOf<AActor> SecondAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	float FirstAmmoChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	float SecondAmmoChance;
};
