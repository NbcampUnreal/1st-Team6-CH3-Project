#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class GETTOJOB_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int MeleeAttack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int WeakAttack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int StrongAttack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int SpecialAttack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int RangeAttack();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int WeakAttack_Rage();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int StrongAttack_Rage();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int SpecialAttack_Rage();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int RangeAttack_Rage();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    int RageMotion();
};
