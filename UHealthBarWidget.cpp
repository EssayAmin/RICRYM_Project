// Fill out your copyright notice in the Description page of Project Settings.


#include "UHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "DamageDummy.h"

void UUHealthBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

  

    // You can initialize anything here if needed
}

void UUHealthBarWidget::UpdateHealthBar(float HealthPercentage)
{
    float percent = HealthPercentage;

    if (HealthBar)
    {
        HealthBar->SetPercent(percent);  // Update progress bar
        UE_LOG(LogTemp, Error, TEXT("Inside Set percet"));
    }
}