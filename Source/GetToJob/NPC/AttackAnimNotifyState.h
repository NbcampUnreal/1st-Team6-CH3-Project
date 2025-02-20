#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackAnimNotifyState.generated.h"

UCLASS()
class GETTOJOB_API UAttackAnimNotifyState : public UAnimNotifyState
{
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	GENERATED_BODY()
	
};
