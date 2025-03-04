#include "NPC/GJNPC.h"
#include "NPC/GJ_AIController.h"
#include "Components/SphereComponent.h"

AGJNPC::AGJNPC() :
	HeadCollisionSphere{ CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollisionSphere")) }
{
	PrimaryActorTick.bCanEverTick = true;
	IsNPCDead = false;
	if (HeadCollisionSphere)
	{
		HeadCollisionSphere->SetupAttachment(GetMesh(), "head_socket");
	}
	FirstAmmoChance = 20.f;
	SecondAmmoChance = 5.f;
}

UBehaviorTree* AGJNPC::GetBehaviorTree() const
{
	return Tree; // Tree
}

APatrolPath* AGJNPC::GetPatrolParth() const
{
	return PatrolPath;
}

UAnimMontage* AGJNPC::GetMontage() const
{
	return Montage;
}

void AGJNPC::SetPatrolPath(APatrolPath* Path)
{
	PatrolPath = Path;
}

void AGJNPC::SetBehaviorTree(UBehaviorTree* ChooseTree)
{
	if (AIControllerClass) // AIController
	{
		AAIController* NPCAIController = Cast<AGJ_AIController>(GetController());
		if (NPCAIController)
		{
			NPCAIController->RunBehaviorTree(ChooseTree);
		}
	}
}

bool AGJNPC::GetNPCDead() const
{
	return IsNPCDead;
}


void AGJNPC::SetNPCDead(bool IsDead)
{
	IsNPCDead = IsDead;
}

void AGJNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGJNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGJNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGJNPC::SpawnDropItem()
{
	const float RandValue = FMath::FRandRange(0.0f, 100.f);
	UWorld* World = GetWorld();
	if (RandValue <= SecondAmmoChance)
	{
		if (SecondAmmo)
		{
			FVector NPCLocation = GetActorLocation();
			FRotator NPCRotation = GetActorRotation();

			NPCLocation.Z -= 90.f;

			World -> SpawnActor<AActor>(SecondAmmo, NPCLocation, NPCRotation);
			UE_LOG(LogTemp, Warning, TEXT("ISSpawned"));
		}
	}
	else if (SecondAmmoChance < RandValue && FirstAmmoChance >= RandValue)
	{
		if (FirstAmmo)
		{
			FVector NPCLocation = GetActorLocation();
			FRotator NPCRotation = GetActorRotation();

			NPCLocation.Z -= 90.f;

			World->SpawnActor<AActor>(FirstAmmo, NPCLocation, NPCRotation);
			UE_LOG(LogTemp, Warning, TEXT("ISSpawned"));
		}
	}
	else return;

	return;
}

int AGJNPC::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
	return 0;
}


