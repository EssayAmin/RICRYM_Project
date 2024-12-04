// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UHealthComponent.generated.h"




UCLASS()
class MYPROJECT3_API AUHealthComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUHealthComponent();

    // Health properties
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float Health;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float MaxHealth;

    // Functions for dynamic initialization and health management
    UFUNCTION(BlueprintCallable, Category = "Health")
    void InitializeHealth(float InitialMaxHealth, float InitialHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

    void SetHealth(float NewHealth);

    // Delegate to broadcast health changes


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    FTimerHandle RegenTimerHandle;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void StartHealthRegen(float RegenRate, float RegenInterval);



public:	
	// Called every frame

};
