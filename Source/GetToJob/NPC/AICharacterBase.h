#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

UCLASS()
class GETTOJOB_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHealth() const;
	float GetAttack() const;
	float GetMaxHealth() const;
	void SetHealth(float NewHealth);
	void SetAttack(float NewAttack);
	void AttackStart() const;
	void AttackEnd() const;
	float MaxHealth{ 10000.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "ture"))
	class UBoxComponent* RightFistCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "ture"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "ture"))
	float Attack;

	bool bCanAttack;
protected:
	virtual void BeginPlay() override;


	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamgeCauser
	) override;

	UFUNCTION()
	void OnAttackOverlapBegin(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherComponent,
		int const OtherBodyIndex,
		bool const FromSeep,
		FHitResult const& SweepResult
	);

	UFUNCTION()
	void OnAttackOverlapEnd(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherComponent,
		int const OtherBodyIndex
	);

	void DestroyActor();

private:
	FTimerHandle AttackCooldownTimer;

	void StartAttackCooldown(float Time);
	void ResetAttackCooldown();
};
