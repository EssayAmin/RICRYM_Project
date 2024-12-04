// Fill out your copyright notice in the Description page of Project Settings.


#include "UPlayerHealthBar.h"
#include "Components/ProgressBar.h"
#include "UObject/ConstructorHelpers.h"

void UUPlayerHealthBar::NativeConstruct()
{
    Super::NativeConstruct();

    // Additional setup can go here, if necessary
}

void UUPlayerHealthBar::UpdateHealthBar(float Health, float MaxHealth)
{
    // Logic to update the health bar (this can be expanded)
    float HealthPercent = Health / MaxHealth;
    UE_LOG(LogTemp, Warning, TEXT("Health Percent: %f"), HealthPercent);
}