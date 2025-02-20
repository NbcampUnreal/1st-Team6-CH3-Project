#include "NPC/GJNPC.h"

AGJNPC::AGJNPC()
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* AGJNPC::GetBehaviorTree() const
{
	return Tree; // Tree 반환 함수
}

APatrolPath* AGJNPC::GetPatrolParth() const
{
	return PatrolPath;
}

UAnimMontage* AGJNPC::GetMontage() const
{
	return Montage;
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


