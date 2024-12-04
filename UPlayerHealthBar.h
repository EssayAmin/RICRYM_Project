// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPlayerHealthBar.generated.h"

/**
 * 
 */

class UProgressBar;

UCLASS()
class MYPROJECT3_API UUPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// Add custom properties or functions here
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealthBar(float Health, float MaxHealth);

protected:

	virtual void NativeConstruct() override;

	// ProgressBar reference for the health bar
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerHealthBar;
	
};
