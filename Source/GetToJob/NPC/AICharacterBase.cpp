#include "NPC/AICharacterBase.h"
#include "NPC/GJNPC.h"
#include "NPC/GJBossNPC.h"
#include "GameManager/GJGameState.h"
#include "GameManager/GJBossGameState.h"
#include "Components/BoxComponent.h"
#include "Character/GJPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Character/GJCharacter.h"
#include "UI/GJHUD.h"

AAICharacterBase::AAICharacterBase():
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
	Health = MaxHealth;
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
			if(AGJGameState* const GameState = Cast<AGJGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				GameState->AddScore(100);
				GameState->AddEnemyKillCount(1);
			}
			if (AGJBossGameState* const GameState = Cast<AGJBossGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				GameState->AddScore(100);
				GameState->AddEnemyKillCount(1);
			}
			NPC->SpawnDropItem();
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
		Boss->PlayHurtSound();

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (AGJHUD* HUD = Cast<AGJHUD>(PlayerController->GetHUD()))
			{
				HUD->UpdateBossHUD(GetHealth(), GetMaxHealth());
			}
		}

		if (Health <= 0)
		{
			Boss->SetIsBossDead(true);
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

			AGJBossGameState* GameState = GetWorld()->GetGameState<AGJBossGameState>();
			if (GameState)
			{
				GameState->SetBossDefeated();
			}
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
			Boss->PlayHeatSound();
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

void AAICharacterBase::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
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