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

int AGJNPC::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
	return 0;
}


