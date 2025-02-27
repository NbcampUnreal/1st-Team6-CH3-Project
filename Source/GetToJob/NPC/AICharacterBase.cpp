#include "NPC/AICharacterBase.h"
#include "NPC/GJNPC.h"
#include "NPC/GJBossNPC.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Character/GJCharacter.h"

AAICharacterBase::AAICharacterBase():
	Health{ 100.f },
	RightFistCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFirstCollisionBox")) }
{
	PrimaryActorTick.bCanEverTick = true;
	bCanAttack = true;
	if (RightFistCollisionBox)
	{
		RightFistCollisionBox->SetupAttachment(GetMesh(), "hand_r_socket");
	}
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAICharacterBase::OnAttackOverlapBegin);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAICharacterBase::OnAttackOverlapEnd);
}

float AAICharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamgeCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount,
		DamageEvent,
		EventInstigator,
		DamgeCauser);
	if (auto const NPC = Cast<AGJNPC>(this))
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
		if (Health <= 0)
		{
			NPC->SetNPCDead(true);
			NPC->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			NPC->GetMesh()->SetSimulatePhysics(true);
			NPC->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

			FTimerHandle DestroyTimerHandle;
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAICharacterBase::DestroyActor, 5.f, false);
		}
	}
	if (auto const Boss = Cast<AGJBossNPC>(this))
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
		if (Health <= 0)
		{
			if (RightFistCollisionBox)
			{
				RightFistCollisionBox->SetCollisionProfileName("NoCollision");
				RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
			}
			Boss->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			Boss->GetMesh()->SetSimulatePhysics(true);
			Boss->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

			FTimerHandle DestroyTimerHandle;
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAICharacterBase::DestroyActor, 5.f, false);
		}
	}
	return ActualDamage;
}

void AAICharacterBase::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSeep, FHitResult const& SweepResult)
{

	if (!bCanAttack) return;
	if (OtherActor == this)
	{
		return;
	}
	if (auto const Player = Cast<AGJCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(
			Player,
			GetAttack(),
			nullptr,
			this,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Error, TEXT("Hitted"));
		if (auto const Boss = Cast<AGJBossNPC>(this))
		{
			StartAttackCooldown(1.f);
		}
		if (auto const NPC = Cast<AGJNPC>(this))
		{
			StartAttackCooldown(0.5f);
		}
	}
}

void AAICharacterBase::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex)
{
}

void AAICharacterBase::DestroyActor()
{
	Destroy();
}

void AAICharacterBase::StartAttackCooldown(float Time)
{
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AAICharacterBase::ResetAttackCooldown, Time);
}

void AAICharacterBase::ResetAttackCooldown()
{
	bCanAttack = true;
}

void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAICharacterBase::GetHealth() const
{
	return Health;
}

float AAICharacterBase::GetAttack() const
{
	return Attack;
}

float AAICharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

void AAICharacterBase::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void AAICharacterBase::SetAttack(float NewAttack)
{
	Attack = NewAttack;
}

void AAICharacterBase::AttackStart() const
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AAICharacterBase::AttackEnd() const
{
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}