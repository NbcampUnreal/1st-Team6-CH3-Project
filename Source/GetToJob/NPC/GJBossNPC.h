#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CombatInterface.h"
#include "NPC/AICharacterBase.h"
#include "PatrolPath.h"
#include "GJBossNPC.generated.h"

UCLASS()
class GETTOJOB_API AGJBossNPC : public AAICharacterBase, public ICombatInterface
{
	GENERATED_BODY()
public:
	AGJBossNPC();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

	UAnimMontage* GetWeakMontage() const;

	UAnimMontage* GetStrongMontage() const;

	UAnimMontage* GetSpecialMontage() const;

	UAnimMontage* GetRangeMontage() const;

	UAnimMontage* GetRageMontage() const;

	void FireProjectile();

	void SetPatrolPath(APatrolPath* Path);

	void SetBehaviorTree(UBehaviorTree* ChooseTree);

	void AttackStart() ;

	void AttackEnd() ;

	bool GetIsFist() const;

	bool GetIsLeft() const;

	bool GetIsRight() const;

	bool GetIsBossDead() const;

	void SetIsFist(bool IsItFist);

	void SetIsRight(bool IsItRight);

	void SetIsLeft(bool IsItLeft);

	void SetIsBossDead(bool BossIsDead);

	int WeakAttack_Implementation() override;

	int StrongAttack_Implementation() override;

	int SpecialAttack_Implementation() override;

	int RangeAttack_Implementation() override;

	int WeakAttack_Rage_Implementation() override;

	int StrongAttack_Rage_Implementation() override;

	int SpecialAttack_Rage_Implementation() override;

	int RangeAttack_Rage_Implementation() override;

	int RageMotion_Implementation() override;

	void PlayHeatSound();

	void PlayHurtSound();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightFootCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftFootCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HeadCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* WeakAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* StrongAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* SpecialAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* RangeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* RageMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HeatSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HurtSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* RageSound;

	USkeletalMeshComponent* SkeletalMeshCom;

	UAnimInstance* AnimInstance;

	virtual void OnAttackOverlapBegin(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherComponent,
		int const OtherBodyIndex,
		bool const FromSweep,
		FHitResult const& SweepResult
	);

	virtual void OnAttackOverlapEnd(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherComponent,
		int const OtherBodyIndex
	);

	bool IsFist;

	bool IsLeft;

	bool IsRight;

	bool IsBossDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float WeakDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float StrongDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float SpecialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AActor> ProjectileClass;

};
