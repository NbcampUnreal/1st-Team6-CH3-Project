#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJ_BossHUDUserWidget.generated.h"

class UProgressBar;


UCLASS()
class GETTOJOB_API UGJ_BossHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

public:
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
};
