// Fill out your copyright notice in the Description page of Project Settings.


#include "UHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values
AUHealthComponent::AUHealthComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ensure health is clamped within valid range
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	
}



void AUHealthComponent::InitializeHealth(float InitialMaxHealth, float InitialHealth)
{
    MaxHealth = InitialMaxHealth;
    Health = FMath::Clamp(InitialHealth, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health Initialized: %f / %f"), Health, MaxHealth);
}

void AUHealthComponent::Heal(float HealAmount)
{
    Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Healed: %f, Current Health: %f"), HealAmount, Health);
    SetHealth(Health);  // Update the character's health
}

void AUHealthComponent::TakeDamage(float DamageAmount)
{
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Took Damage: %f, Current Health: %f"), DamageAmount, Health);
    SetHealth(Health);  // Update the character's health
}

void AUHealthComponent::StartHealthRegen(float RegenRate, float RegenInterval)
{
    // Start a timer to regenerate health periodically
    GetWorld()->GetTimerManager().SetTimer(
        RegenTimerHandle,
        [this, RegenRate]()
        {
            Heal(RegenRate);
            if (Health >= MaxHealth)
            {
                GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle); // Stop regeneration at max health
            }
        },
        RegenInterval,
        true // Loop indefinitely until manually stopped
    );
}

void AUHealthComponent::SetHealth(float NewHealth)
{
    // Clamp health between 0 and MaxHealth
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

}



