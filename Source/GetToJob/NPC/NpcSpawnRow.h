#pragma once

#include "CoreMinimal.h"
#include "NPC/GJNPC.h"
#include "NPCSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FNPCSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGJNPC> NPCClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
