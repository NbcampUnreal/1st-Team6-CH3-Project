#include "NPC/GJNPC.h"
#include "NPC/GJ_AIController.h"

AGJNPC::AGJNPC()
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* AGJNPC::GetBehaviorTree() const
{
	return Tree; // Tree ��ȯ �Լ�
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
	if (AIControllerClass) // AIController�� �����ϴ� ��쿡�� ����
	{
		AAIController* NPCAIController = Cast<AGJ_AIController>(GetController());
		if (NPCAIController)
		{
			NPCAIController->RunBehaviorTree(ChooseTree);
		}
	}
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


