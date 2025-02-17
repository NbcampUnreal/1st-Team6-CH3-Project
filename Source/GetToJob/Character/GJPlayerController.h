#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GJPlayerController.generated.h"

class UInputMappingContext; // IMC ���� ���� ����
class UInputAction; // IA ���� ���� ����

UCLASS()
class GETTOJOB_API AGJPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGJPlayerController();

	// �����Ϳ��� ������ IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// IA_Move�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Jump�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	// IA_Look�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Sprint�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	// IA_Sit�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SitAction;

	virtual void BeginPlay() override;

};


