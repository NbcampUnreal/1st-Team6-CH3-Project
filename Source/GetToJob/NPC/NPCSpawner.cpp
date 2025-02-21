#include "NPC/NPCSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"

ANPCSpawner::ANPCSpawner()
{
 	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ChoicePath = nullptr;
}

void ANPCSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (NPCClass)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ANPCSpawner::SpawnNPCs, SpawnInterval, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCClass not set!"));
	}
}

void ANPCSpawner::SpawnNPCs()
{
    if (!NPCClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCClass not set!"));
        return;
    }

    for (int32 i = 0; i < NumberOfActorsToSpawn; ++i)
    {
        AGJNPC* SpawnedNPC = SpawnNPC();
        if (SpawnedNPC)
        {
            APatrolPath* FoundPath = FindPatrolPath();
            if (FoundPath)
            {
                SpawnedNPC->SetPatrolPath(FoundPath); // PatrolPath 할당
                UE_LOG(LogTemp, Warning, TEXT("PatrolPath Found!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("PatrolPath Not Found!"));
            }
            if (ChoicePath) // 변경할 Behavior Tree가 설정되었는지 확인
            {
                SpawnedNPC->SetBehaviorTree(ChoicePath);
                UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Changed!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("BT_NPC_RandomPath not set!"));
            }
        }
    }
}

AGJNPC* ANPCSpawner::SpawnNPC()
{
    FVector SpawnLocation = GetActorLocation(); // 스포너 위치
    FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    return GetWorld()->SpawnActor<AGJNPC>(NPCClass, SpawnLocation, SpawnRotation, SpawnParams);
}

APatrolPath* ANPCSpawner::FindPatrolPath()
{
    for (TActorIterator<APatrolPath> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        return *ActorItr; // 월드에 있는 PatrolPath 액터 반환 (가장 먼저 찾은 액터)
    }
    return nullptr; // 찾지 못했을 경우 nullptr 반환
}


