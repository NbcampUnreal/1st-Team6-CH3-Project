#include "NPC/GJBossNPC.h"
#include "NPC/GJBossProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GJBossAIController.h"

AGJBossNPC::AGJBossNPC() :
	RightFootCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootCollisionBox")) },
	LeftFootCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootCollisionBox")) },
	HeadCollisionSphere{ CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollisionSphere"))}
{
	if (RightFootCollisionBox)
	{
		RightFootCollisionBox->SetupAttachment(GetMesh(), "foot_r_socket");
	}
	if (LeftFootCollisionBox)
	{
		LeftFootCollisionBox->SetupAttachment(GetMesh(), "foot_l_socket");
	}
	if (HeadCollisionSphere)
	{
		HeadCollisionSphere->SetupAttachment(GetMesh(), "head_socket");
	}
	IsFist = false;
	IsRight = false;
	IsLeft = false;
	IsBossDead = false;
}

void AGJBossNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AGJBossNPC::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* AGJBossNPC::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* AGJBossNPC::GetWeakMontage() const
{
	return WeakAttackMontage;
}

UAnimMontage* AGJBossNPC::GetStrongMontage() const
{
	return StrongAttackMontage;
}

UAnimMontage* AGJBossNPC::GetSpecialMontage() const
{
	return SpecialAttackMontage;
}

UAnimMontage* AGJBossNPC::GetRangeMontage() const
{
	return RangeAttackMontage;
}

UAnimMontage* AGJBossNPC::GetRageMontage() const
{
	return RageMontage;
}

void AGJBossNPC::FireProjectile()
{
	if (ProjectileClass)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter)
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector NPCLocation = GetActorLocation();
			FVector LaunchDirection = (PlayerLocation - NPCLocation).GetSafeNormal();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigatorController()->GetPawn();


			UE_LOG(LogTemp, Warning, TEXT("FireProjectile: Instigator=%s"), *GetNameSafe(SpawnParams.Instigator));

			AGJBossProjectile* Projectile = GetWorld()->SpawnActor<AGJBossProjectile>(ProjectileClass, NPCLocation, LaunchDirection.Rotation(), SpawnParams);

			if (Projectile)
			{
				UE_LOG(LogTemp, Warning, TEXT("It Spawnned"));
				Projectile->ProjectileComponent->Velocity = LaunchDirection * Projectile->ProjectileComponent->InitialSpeed;
			}
		}
	}
}

void AGJBossNPC::SetPatrolPath(APatrolPath* Path)
{
	PatrolPath = Path;
}

void AGJBossNPC::SetBehaviorTree(UBehaviorTree* ChooseTree)
{

	if (AIControllerClass) // AIController가 존재하는 경우에만 변경
	{
		AAIController* BossAIController = Cast<AGJBossAIController>(GetController());
		if (BossAIController)
		{
			BossAIController->RunBehaviorTree(ChooseTree);
		}
	}
}

void AGJBossNPC::AttackStart() 
{
	if (GetIsFist()) {
		Super::AttackStart();
		SetAttack(WeakDamage);
	}
	if (RightFootCollisionBox)
	{
		if (GetIsRight())
		{
			RightFootCollisionBox->SetCollisionProfileName("Fist");
			RightFootCollisionBox->SetNotifyRigidBodyCollision(true);
			SetAttack(StrongDamage);
		}
	}
	if (LeftFootCollisionBox)
	{
		if (GetIsLeft())
		{
			LeftFootCollisionBox->SetCollisionProfileName("Fist");
			LeftFootCollisionBox->SetNotifyRigidBodyCollision(true);
			SetAttack(SpecialDamage);
		}
	}
}

void AGJBossNPC::AttackEnd() 
{
	Super::AttackEnd();
	if (RightFootCollisionBox)
	{
		RightFootCollisionBox->SetCollisionProfileName("NoCollision");
		RightFootCollisionBox->SetNotifyRigidBodyCollision(false);
	}
	if (LeftFootCollisionBox)
	{
		LeftFootCollisionBox->SetCollisionProfileName("NoCollision");
		LeftFootCollisionBox->SetNotifyRigidBodyCollision(false);
	}
}

bool AGJBossNPC::GetIsFist() const
{
	return IsFist;
}

bool AGJBossNPC::GetIsRight() const
{
	return IsRight;
}

bool AGJBossNPC::GetIsBossDead() const
{
	return IsBossDead;
}

bool AGJBossNPC::GetIsLeft() const
{
	return IsLeft;
}

void AGJBossNPC::SetIsFist(bool IsItFist)
{
	IsFist = IsItFist;
}

void AGJBossNPC::SetIsRight(bool IsItRight)
{
	IsRight = IsItRight;
}

void AGJBossNPC::SetIsLeft(bool IsItLeft)
{
	IsLeft = IsItLeft;
}

void AGJBossNPC::SetIsBossDead(bool BossIsDead)
{
	IsBossDead = BossIsDead;
}

int AGJBossNPC::WeakAttack_Implementation()
{
	if (WeakAttackMontage)
	{
		PlayAnimMontage(WeakAttackMontage);
	}
	return 0;
}

int AGJBossNPC::StrongAttack_Implementation()
{
	if (StrongAttackMontage)
	{
	}
	return 0;
}

int AGJBossNPC::SpecialAttack_Implementation()
{
	if (SpecialAttackMontage)
	{
		PlayAnimMontage(SpecialAttackMontage);

	}
	return 0;
}

int AGJBossNPC::RangeAttack_Implementation()
{
	if (RangeAttackMontage)
	{
		PlayAnimMontage(RangeAttackMontage);
	}
	return 0;
}

int AGJBossNPC::WeakAttack_Rage_Implementation()
{
	if (WeakAttackMontage)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(WeakAttackMontage, 1.5f);
		}
		else PlayAnimMontage(WeakAttackMontage);
	}
	return 0;
}

int AGJBossNPC::StrongAttack_Rage_Implementation()
{
	if (StrongAttackMontage)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(StrongAttackMontage, 1.5f);
		}
		else PlayAnimMontage(StrongAttackMontage);
	}
	return 0;
}

int AGJBossNPC::SpecialAttack_Rage_Implementation()
{
	if (SpecialAttackMontage)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(SpecialAttackMontage, 1.5f);
		}
		else PlayAnimMontage(SpecialAttackMontage);
	}
	return 0;
}

int AGJBossNPC::RangeAttack_Rage_Implementation()
{
	if (RangeAttackMontage)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(RangeAttackMontage, 1.5f);
		}
		else PlayAnimMontage(RangeAttackMontage);
	}
	return 0;
}

int AGJBossNPC::RageMotion_Implementation()
{
	if (RageMontage)
	{
		PlayAnimMontage(RageMontage);
		if (RageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				RageSound,
				GetActorLocation()
			);
		}
	}
	return 0;
}

void AGJBossNPC::PlayHeatSound()
{
	if (HeatSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HeatSound,
			GetActorLocation()
		);
	}
}

void AGJBossNPC::PlayHurtSound()
{
	if (HurtSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HurtSound,
			GetActorLocation()
		);
	}
}


void AGJBossNPC::BeginPlay()
{
	Super::BeginPlay();

	RightFootCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGJBossNPC::OnAttackOverlapBegin);
	RightFootCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGJBossNPC::OnAttackOverlapEnd);
	LeftFootCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGJBossNPC::OnAttackOverlapBegin);
	LeftFootCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGJBossNPC::OnAttackOverlapEnd);

	SkeletalMeshCom = this->GetMesh();
	AnimInstance = SkeletalMeshCom->GetAnimInstance();
}

void AGJBossNPC::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	Super::OnAttackOverlapBegin(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, FromSweep, SweepResult);
}

void AGJBossNPC::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex)
{
	Super::OnAttackOverlapEnd(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
}
