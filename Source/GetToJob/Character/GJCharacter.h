#pragma once

#include "CoreMinimal.h"
#include "NPC/AICharacterBase.h"
#include "GameFramework/Character.h"
#include "Character/GJInventoryComponent.h"
#include "Character/GJDebuffComponent.h" 
#include "GJCharacter.generated.h"

class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언
struct FInputActionValue; // Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
class AGJBaseGun;
class AGJMiniGun; // 미니건 클래스 선언

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Revolver UMETA(DisplayName = "Revolver"),
	Rifle UMETA(DisplayName = "Rifle"),
	RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	MiniGun UMETA(DisplayName = "MiniGun")
};

UCLASS()
class GETTOJOB_API AGJCharacter : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AGJCharacter();

	void SetSpeed(float NewSpeedMultiplier);

	// 이동 속도 관련 프로퍼티들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed; // 앉은 상태 속도

	// 디버프 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	UGJDebuffComponent* DebuffComponent;

	// 디버프 적용 함수
	void ApplyDebuff(const FDebuffEffect& Debuff);

	// 디버프 컴포넌트 Getter
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	UGJDebuffComponent* GetDebuffComponent() const { return DebuffComponent; }

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	AGJHealingItem* InteractableHealingItem;

	void UseHealingItem();

	// 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
	AGJBaseGun* CurrentGun;

	// 상호작용 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Interact();

	// 인벤토리 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UGJInventoryComponent* InventoryComponent;

	// 무기 장착
	void EquipWeapon(AGJBaseGun* NewWeapon);

	// 이전 무기 상태 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType PreviousWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType CurrentWeaponType;

	void UpdateWeaponState(AGJBaseGun* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

	virtual void Tick(float Deltatime) override;
	float LastSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	class AGJBaseGun* InteractableWeapon;

	// 미니건 클래스
	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<class AGJMiniGun> MiniGunClass; // 미니건 클래스를 에디터에서 설정 가능하게

	// 현재 미니건 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	AGJMiniGun* MiniGun;

	// 궁극기 사용
	void ActivateUltimateWeapon();

protected:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamgeCauser
	) override;

	// 무기 장착 해제
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipCurrentWeapon();

	// 무기 장착 요청(인벤토리에서)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeaponFromInventory(int32 SlotIndex);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float LookSensitivity; // 마우스 감도 조절을 위한 변수

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BackwardSpeedMultiplier;  // 후진 속도 배율

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpCooldownTime = 0.5f; // 점프 쿨다운 시간

	FTimerHandle JumpCooldownTimer; // 점프 쿨다운 타이머 핸들

	bool bCanJump = true; // 점프 가능 여부

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void ResetJump(); // 쿨다운이 끝난 후 점프를 다시 가능하게 만드는 함수

	// 사운드 재생 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* DeathSound;

	// 테스트용 죽이기
	UFUNCTION(BlueprintCallable, Category = "Test")
	void TriggerDeathTest();

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
