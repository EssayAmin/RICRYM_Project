// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "DamageDummy.generated.h"

class AUHealthComponent;
class AMyProject3Character;
class UUHealthBarWidget;
class UWidgetComponent;
class ADamageDummyAIController;


UCLASS()
class MYPROJECT3_API ADamageDummy : public APawn
{
	GENERATED_BODY()

    
	
public:	
	ADamageDummy();

    UFUNCTION(BlueprintCallable, Category = "Event")
    void TakeDamage(float DamageAmount, AActor* InstigatorActor);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionComponent;

    FVector InitialSpawnLocation;
    FRotator InitialSpawnRotation;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
    float RespawnDelay;


    


    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    FTimerHandle DestructionTimerHandle;

    FTimerHandle RespawnTimerHandle;

    void DestroyDummy();

    void Respawn();

    AActor* LastAttacker;  // Keeps track of who attacked this dummy
   

public:	

    // Health component
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    AUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* Hitbox;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float SetCurrentHealth(float CurrentHealth, float MaxHealth);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimSequence* DeathAnim;

    UPROPERTY()
    UUHealthBarWidget* HealthBarWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    TSubclassOf<UUserWidget> HealthBarWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UProgressBar* EnemyHealthBar;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* MyWidgetComponent;

    // Collision Begin Event
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);




};
