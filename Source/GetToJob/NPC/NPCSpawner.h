// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/GJNPC.h"
#include "NPCSpawner.generated.h"

class UBoxComponent;

UCLASS()
class GETTOJOB_API ANPCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCSpawner();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    virtual void BeginPlay() override;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<AGJNPC> NPCClass; // 스폰할 NPC 클래스

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0.0"))
    float SpawnInterval = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "1"))
    int32 NumberOfActorsToSpawn = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBehaviorTree* ChoicePath; // 추가된 부분

    FTimerHandle SpawnTimerHandle;

    UFUNCTION()
    void SpawnNPCs();

    AGJNPC* SpawnNPC();

    APatrolPath* FindPatrolPath(); // PatrolPath 찾는 함수
};
