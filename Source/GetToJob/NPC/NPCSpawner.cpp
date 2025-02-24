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
	if (NPCDataTable)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ANPCSpawner::SpawnNPCs, SpawnInterval, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCClass not set!"));
	}
}

AGJNPC* ANPCSpawner::SpawnRandomNPC()
{
    if (FNPCSpawnRow* SelectedRow = GetRandomNPC())
    {
        if (UClass* ActualClass = SelectedRow->NPCClass.Get())
        {
            return SpawnNPC(ActualClass);
        }
    }
    return nullptr;
}

FNPCSpawnRow* ANPCSpawner::GetRandomNPC() const
{
    if (!NPCDataTable) return nullptr;

    TArray<FNPCSpawnRow*> AllRows;
    static const FString ContextString(TEXT("NPCSpawnContext"));

    NPCDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    float TotalChance = 0.0f;
    for (const FNPCSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FNPCSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

void ANPCSpawner::SpawnNPCs()
{
    if (!NPCDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCDataTable not set!"));
        return;
    }

    for (int32 i = 0; i < NumberOfActorsToSpawn; ++i)
    {
        AGJNPC* SpawnedNPC = SpawnRandomNPC();
        if (SpawnedNPC)
        {
            APatrolPath* FoundPath = FindPatrolPath();
            if (FoundPath)
            {
                SpawnedNPC->SetPatrolPath(FoundPath); // PatrolPath �Ҵ�
                UE_LOG(LogTemp, Warning, TEXT("PatrolPath Found!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("PatrolPath Not Found!"));
            }
            if (ChoicePath) // ������ Behavior Tree�� �����Ǿ����� Ȯ��
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

AGJNPC* ANPCSpawner::SpawnNPC(TSubclassOf<AGJNPC> NPCClass)
{
    FVector SpawnLocation = GetActorLocation(); // ������ ��ġ
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
        return *ActorItr; // ���忡 �ִ� PatrolPath ���� ��ȯ (���� ���� ã�� ����)
    }
    return nullptr; // ã�� ������ ��� nullptr ��ȯ
}


