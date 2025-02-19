#include "NPC/GJNPC.h"

AGJNPC::AGJNPC()
{
	PrimaryActorTick.bCanEverTick = true;

}

UBehaviorTree* AGJNPC::GetBehaviorTree() const
{
	return Tree; // Tree 반환 함수
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

