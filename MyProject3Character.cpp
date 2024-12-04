// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject3Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "projectile.h"
#include "UHealthComponent.h"
#include "UPlayerHealthBar.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "DamageDummy.h" 
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/EngineTypes.h"
#include "Components/PrimitiveComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProject3Character

AMyProject3Character::AMyProject3Character()
{

	TargetDummyClass = ADamageDummy::StaticClass();


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	// Initialize the health component
	HealthComponent = CreateDefaultSubobject<AUHealthComponent>(TEXT("HealthComponent"));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/HealthBar"));
	if (WidgetClassFinder.Succeeded())
	{
		HealthBarWidgetClass = WidgetClassFinder.Class;
	}
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);


	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	Experience = 20.f;
	Level = 1;
	ExpToLevelUp = 100.f;  // Modify this based on your preferred leveling system

	SetPlayerRole(EPlayerRole::Warrior);

	Health = MaxHealth;

	AttackCooldownTime = 1.0f;
	bCanAttack = true;


	static ConstructorHelpers::FObjectFinder<UAnimSequence> anim(TEXT("/Script/Engine.AnimSequence'/Game/RamsterZ_FreeAnims_Volume1/AnimationSequence/H2H/H2H_PunchCombo01.H2H_PunchCombo01'"));
	PunchCombo01 = anim.Object;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Initialize the health with unique values for this character
	if (HealthComponent)
	{
		HealthComponent->InitializeHealth(MaxHealth, Health); // MaxHealth = 200, InitialHealth = 150
	}

	HealthBarWidget1 = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidgetClass);
	if (HealthBarWidget1)
	{
		HealthBarWidget1->AddToViewport();
	}

}

void AMyProject3Character::BeginPlay()
{
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AutoAimAtTarget(DeltaTime);
}

void AMyProject3Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyProject3Character::AutoAimAtTarget(float DeltaTime)
{
	// Find the closest target
	CurrentTarget = FindClosestTarget();

	if (CurrentTarget)
	{
		// Calculate the direction to the target
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector DirectionToTarget = TargetLocation - GetActorLocation();

		// Convert direction to rotation
		FRotator TargetRotation = DirectionToTarget.Rotation();

		FRotator NewControlRotation = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 10.0f); // Adjust the speed here

	}
}

void AMyProject3Character::SetPlayerRole(EPlayerRole ARole)
{

	PlayerRole = ARole;
	switch (ARole)
	{
	case EPlayerRole::Warrior:
		MaxHealth = 200.0f;
		Damage = 25.0f;

		break;

	case EPlayerRole::Mage:
		MaxHealth = 100.0f;
		Damage = 50.0f;

		break;

	default:
		MaxHealth = 200.0f;
		Damage = 25.0f;
		break;
	}
}

void AMyProject3Character::GainExperience(float Amount)
{
	Experience += Amount;

	// Check if we have enough experience to level up
	if (Experience >= ExpToLevelUp)
	{
		LevelUp();
	}
}

void AMyProject3Character::LevelUp()
{
	Level++;  // Increase the level
	Experience -= ExpToLevelUp;  // Subtract the amount required for leveling up
	ExpToLevelUp *= 1.2f;  // Increase the required EXP for the next level

	UE_LOG(LogTemp, Warning, TEXT("Character leveled up to level %d!"), Level);

	// Update the level
	UpdateLevel();
}

void AMyProject3Character::UpdateLevel()
{

	float NewHealth = MaxHealth + (Level * 10);  // Increase health by 10 per level
	MaxHealth = NewHealth;

	UE_LOG(LogTemp, Warning, TEXT("Character's new level: %d"), Level);
}

AActor* AMyProject3Character::FindClosestTarget()
{

	FVector PlayerLocation = GetActorLocation();
	AActor* ClosestTarget = nullptr;
	float MinDistance = AutoAimRange;

	// Search for Damage Dummies instead of generic actors
	TArray<AActor*> PotentialTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetDummyClass, PotentialTargets);

	if (PotentialTargets.Num() == 0)
	{
		if (GEngine)
		{
			/*GEngine->AddOnScreenDebugMessage(
				-1, 5.0f, FColor::Red,
				TEXT("No targets of TargetDummyClass found in the world!")
			);*/
		}
		return nullptr;
	}

	for (AActor* Target : PotentialTargets)
	{
		
		float Distance = FVector::Dist(PlayerLocation, Target->GetActorLocation());

		/* Debugging: Draw a line to each target and log distance
		DrawDebugLine(
			GetWorld(),
			PlayerLocation,
			Target->GetActorLocation(),
			FColor::Blue,   // Color of the line
			false,          // Do not persist
			2.0f,           // Duration (2 seconds)
			0,
			2.0f            // Thickness 
		);*/

		// Debugging: Print the target's name and distance
		if (GEngine)
		{
			/*GEngine->AddOnScreenDebugMessage(
				-1, 2.0f, FColor::Yellow,
				FString::Printf(TEXT("Target: %s, Distance: %.2f"),
					*Target->GetName(), Distance)
			);*/
		}

		if (Distance < MinDistance)
		{
			ClosestTarget = Target;
			MinDistance = Distance;
		}
	}

	return ClosestTarget;
}

void AMyProject3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)


{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject3Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject3Character::Look);

		// BAsic Attack
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AMyProject3Character::StartBasicAttack);

		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProject3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProject3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProject3Character::StartBasicAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
	UE_LOG(LogTemp, Warning, TEXT("we are using basic attack"));

	BasicAttackUsed = true;
	
	bCanAttack = false;

	// Check for overlapping actors in the player's range
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ADamageDummy::StaticClass());  // Only check for DamageDummy act

	switch (PlayerRole)
	{
	case EPlayerRole::Warrior:
		
		for (AActor* Actor : OverlappingActors)
		{
			ADamageDummy* Dummy = Cast<ADamageDummy>(Actor);

			if (Dummy)
			{
				// Apply damage to the dummy
				Dummy->TakeDamage(Damage, this);  // 10 is the damage amount

			}
		};

		break;

	case EPlayerRole::Mage:
		
		if (CurrentTarget)
		{

			UE_LOG(LogTemp, Warning, TEXT("current current"));

			// Aim directly at the target
			FVector StartLocation = GetActorLocation();  // Start point of the projectile
			FVector TargetLocation;

			UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(CurrentTarget->GetRootComponent());
			if (RootComp)
			{
				TargetLocation = RootComp->Bounds.Origin;  // Center of the actor's collision bounds
			}
			else
			{
				TargetLocation = CurrentTarget->GetActorLocation() + FVector(0, 0, 50);  // Default offset if no collision bounds exist
			}

			// Calculate the direction to the target
			FRotator FireDirection = (TargetLocation - StartLocation).Rotation();



			// Spawn the projectile (replace SpawnProjectile with your own implementation)
			GetWorld()->SpawnActor<Aprojectile>(ProjectileAttack, GetActorLocation(), FireDirection);


		}
		else
		{
			GetWorld()->SpawnActor<Aprojectile>(ProjectileAttack, GetActorLocation() + GetActorForwardVector() * 100.0f, GetActorRotation());

		}

		break;

	default:
		
		for (AActor* Actor : OverlappingActors)
		{
			ADamageDummy* Dummy = Cast<ADamageDummy>(Actor);

			if (Dummy)
			{
				// Apply damage to the dummy
				Dummy->TakeDamage(Damage, this);

			}
		};

		break;
	}

	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &AMyProject3Character::ResetAttackCooldown, AttackCooldownTime, false);
	

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack on cooldown"));
	}
	
}

void AMyProject3Character::TakeDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("we are taking damage "), DamageAmount);
	playerHealth -= DamageAmount;

	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
	}
}

void AMyProject3Character::ResetAttackCooldown()
{
	bCanAttack = true;  // Cooldown is over, allow another attack
	UE_LOG(LogTemp, Warning, TEXT("Attack cooldown over"));
}


void AMyProject3Character::ApplyAOEHealing(float HealAmount, float Radius, FVector Origin)
{
	// Create a list to hold hit results
	TArray<FHitResult> HitResults;

	HealthComponent->Heal(HealAmount);

	UE_LOG(LogTemp, Warning, TEXT("healing "));

	Health = HealthComponent ? HealthComponent->Health : 0.f;


}