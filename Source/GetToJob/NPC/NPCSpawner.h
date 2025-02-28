// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/GJNPC.h"
#include "NPC/NPCSpawnRow.h"
#include "NPCSpawner.generated.h"

class UBoxComponent;

UCLASS()
class GETTOJOB_API ANPCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ANPCSpawner();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* NPCDataTable;
protected:


    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0.0"))
    float AiAttackDamage = 10.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0.0"))
    float SpawnInterval = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "1"))
    int32 NumberOfActorsToSpawn = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBehaviorTree* ChoicePath; // 추가된 부분

    FTimerHandle SpawnTimerHandle;


    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AGJNPC* SpawnRandomNPC();
    FNPCSpawnRow* GetRandomNPC() const;

    UFUNCTION()
    void SpawnNPCs();

    AGJNPC* SpawnNPC(TSubclassOf<AGJNPC> NPCClass);

    APatrolPath* FindPatrolPath(); // PatrolPath 찾는 함수
};
