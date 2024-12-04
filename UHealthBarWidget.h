// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UHealthBarWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class ADamageDummy;

UCLASS()
class MYPROJECT3_API UUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* HealthBar;

    // Function to update the progress bar
    UFUNCTION(BlueprintCallable)
    void UpdateHealthBar(float HealthPercentage);


protected:
    // Override NativeConstruct to initialize the widget
    virtual void NativeConstruct() override;

    

};
