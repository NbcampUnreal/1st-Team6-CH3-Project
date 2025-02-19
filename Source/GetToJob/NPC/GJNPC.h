#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GJNPC.generated.h"

UCLASS()
class GETTOJOB_API AGJNPC : public ACharacter
{
	GENERATED_BODY()

public:
	AGJNPC();

	UBehaviorTree* GetBehaviorTree() const; //Tree Return 함수

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrviateAccess = "true")) // UPROPERTY 로 지정 가능한 BehaviorTree 인스턴스 생성
	UBehaviorTree* Tree;
};
