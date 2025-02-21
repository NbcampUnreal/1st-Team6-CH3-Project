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

	UBehaviorTree* GetBehaviorTree() const; //Tree Return 함수

	APatrolPath* GetPatrolParth() const;

	UAnimMontage* GetMontage() const;

	void SetPatrolPath(APatrolPath* Path);

	void SetBehaviorTree(UBehaviorTree* ChooseTree);

	int MeleeAttack_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true")) // UPROPERTY 로 지정 가능한 BehaviorTree 인스턴스 생성
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrviateAccess = "true"))
	UAnimMontage* Montage;
};
