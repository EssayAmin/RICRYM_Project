// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageDummy.h"
#include "UHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "MyProject3Character.h"
#include "UHealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "DamageDummyAIController.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"


ADamageDummy::ADamageDummy()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkeletalMeshComp;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<AUHealthComponent>(TEXT("HealthComponent"));

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));

	MyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MyWidgetComponent"));
	MyWidgetComponent->SetupAttachment(RootComponent); 
	MyWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
	MyWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); 

	Hitbox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ADamageDummy::OnOverlapBegin);

	InitialSpawnLocation = GetActorLocation();
	InitialSpawnRotation = GetActorRotation();

	RespawnDelay = 5.0f;

	LastAttacker = nullptr;  // Default attacker

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

}


void ADamageDummy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->InitializeHealth(MaxHealth, CurrentHealth); 
		UE_LOG(LogTemp, Warning, TEXT("Current Health Updated, %f"), CurrentHealth);
	}

	SetCurrentHealth(CurrentHealth, MaxHealth);

	ADamageDummyAIController* AIController = Cast<ADamageDummyAIController>(GetController());


	if (!AIController)
	{
	
		AIController = GetWorld()->SpawnActor<ADamageDummyAIController>(ADamageDummyAIController::StaticClass());

		AIController->Possess(this);
	}
}


void ADamageDummy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		
	}
}


// Check if the actor is dead
bool ADamageDummy::IsDead() const
{
    return CurrentHealth <= 0.f;
}

// Function to apply damage
void ADamageDummy::TakeDamage(float DamageAmount, AActor* InstigatorActor)
{
	

	if (HealthComponent)
	{
		TArray<FHitResult> HitResults;

		LastAttacker = InstigatorActor;

		HealthComponent->TakeDamage(DamageAmount);

		CurrentHealth = HealthComponent->Health;

		UE_LOG(LogTemp, Warning, TEXT("Current Health Updated, %f"), CurrentHealth);
		if (HealthComponent)
		{
			HealthComponent->InitializeHealth(MaxHealth, CurrentHealth); 
		}

		SetCurrentHealth(CurrentHealth,MaxHealth);

		if (CurrentHealth <= 0.0f)
		{
			if (SkeletalMeshComp && DeathAnim)
			{
				SkeletalMeshComp->PlayAnimation(DeathAnim, false);
			}

			// Delay destruction until the animation finishes
			GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ADamageDummy::DestroyDummy, 2.0f, false); 

			IsDead();
		}
		

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent is nullptr!"));
	}

  
}

void ADamageDummy::DestroyDummy()
{

	AMyProject3Character* Player = Cast<AMyProject3Character>(LastAttacker);

	UE_LOG(LogTemp, Warning, TEXT("Dummy destroyed! Respawning..."));

	// Start respawn timer
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ADamageDummy::Respawn, 4.0f, false );

	if (Player)
	{
		Player->GainExperience(25.f);
	}


	// Destroy the dummy actor after the animation
	//Destroy();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

}

void ADamageDummy::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn function called!"));

	// Reset health
	CurrentHealth = MaxHealth;

	if (HealthComponent)
	{
		HealthComponent->InitializeHealth(MaxHealth, CurrentHealth); 
		UE_LOG(LogTemp, Warning, TEXT("Current Health Updated, %f"), CurrentHealth);
	}

	SetCurrentHealth(CurrentHealth, MaxHealth);

	// Make the actor visible and active again
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

}

float ADamageDummy::SetCurrentHealth(float NewCurrentHealth, float NewMaxHealth)
{
	float HealthPercentage = NewCurrentHealth / NewMaxHealth;

	if (MyWidgetComponent)
	{
		UUserWidget* Widget = MyWidgetComponent->GetUserWidgetObject();
		if (Widget)
		{
			HealthBarWidget = Cast<UUHealthBarWidget>(Widget);
			if (HealthBarWidget)
			{
				HealthBarWidget->UpdateHealthBar(HealthPercentage);
			}
		}
	}
	return CurrentHealth;

}

