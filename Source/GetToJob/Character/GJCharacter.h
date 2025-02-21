﻿#pragma once

#include "CoreMinimal.h"
#include "NPC/AICharacterBase.h"
#include "GameFramework/Character.h"
#include "GJCharacter.generated.h"

class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언
struct FInputActionValue; // Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
class AGJBaseGun;

UCLASS()
class GETTOJOB_API AGJCharacter : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AGJCharacter();

	// 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
	AGJBaseGun* CurrentGun;


protected:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamgeCauser
	) override;



	// 무기 발사 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	//무기 재장전 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReloadWeapon();

	// 무기 버리기 함수
	UFUNCTION()
	void DropWeapon();  // 무기 버리기 함수

	/*virtual void Tick(float DeltaTime) override;*/
	virtual void BeginPlay() override;

	// 이동 속도 관련 프로퍼티들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed; // 앉은 상태 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float LookSensitivity; // 마우스 감도 조절을 위한 변수

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BackwardSpeedMultiplier;  // 후진 속도 배율


	// 입력 바인딩을 처리할 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IA_Move와 IA_Jump 등을 처리할 함수 원형
	// Enhanced Input에서 액션 값은 FInputActionValue로 전달
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartSit(const FInputActionValue& value);
	UFUNCTION()
	void StopSit(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnDeath();

	bool bIsDead = false;
};
