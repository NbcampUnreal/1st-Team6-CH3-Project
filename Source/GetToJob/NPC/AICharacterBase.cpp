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
	RightFistCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFirstCollisionBox")) },
	Attack{10.f}
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

		}
	}
	if (auto const NPC = Cast<AGJBossNPC>(this))
	{
		if (Health <= 0)
		{
			if (RightFistCollisionBox)
			{
				RightFistCollisionBox->SetCollisionProfileName("NoCollision");
				RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
			}
			NPC->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			NPC->GetMesh()->SetSimulatePhysics(true);
			NPC->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

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
			Attack,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Error, TEXT("Hitted"));
		StartAttackCooldown();
	}
}

void AAICharacterBase::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex)
{
}

void AAICharacterBase::StartAttackCooldown()
{
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AAICharacterBase::ResetAttackCooldown, 0.5f);
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

void AAICharacterBase::SetHealth(float const NewHealth)
{
	Health = NewHealth;
}

void AAICharacterBase::SetAttack(float const NewAttack)
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