// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UPlayerHealthBar.h"
#include "MyProject3Character.generated.h"



class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AUHealthComponent;
class UUPlayerHealthBar;
class ADamageDummy;
struct FInputActionValue;

class UUserWidget;
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Mage UMETA(DisplayName = "Mage"),
};

UCLASS(config=Game)
class AMyProject3Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Basic Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BasicAttackAction;

	

	



public:
	AMyProject3Character();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class Aprojectile> ProjectileAttack;

	// Health component for managing this character's health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	AUHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	EPlayerRole PlayerRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	UParticleSystem* HealingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoAim")
	float AutoAimRange = 1000.0f;

	// Class of the target actors (e.g., Enemy class)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoAim")
	TSubclassOf<AActor> TargetActorClass;

	// Store the currently locked target
	AActor* CurrentTarget;

	AActor* FindClosestTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoAim")
	TSubclassOf<AActor> TargetClass;  // Filter to find specific actors (e.g., enemies)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoAim")
	TSubclassOf<ADamageDummy> TargetDummyClass;

	void AutoAimAtTarget(float DeltaTime);

	UPROPERTY()
	ADamageDummy* DamageDummyActor;


	// Experience points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	float Experience;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	float Damage;

	// Level of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 Level;

	// Experience required to level up
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float ExpToLevelUp;

	// Function to handle gaining experience
	void GainExperience(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Role")
	void SetPlayerRole(EPlayerRole ARole);

	// Function to handle leveling up
	void LevelUp();

	UPROPERTY(EditAnywhere)
	class UAnimSequence* PunchCombo01;

	UFUNCTION(BlueprintCallable, Category = "AOE")
	void ApplyAOEHealing(float HealAmount, float Radius, FVector Origin);

	float AttackCooldownTime;

	bool bCanAttack;

	void ResetAttackCooldown();

	FTimerHandle AttackCooldownTimerHandle;




private:

	
	
	UPROPERTY()
	UUPlayerHealthBar* HealthBarWidget;

	// Function to level up the character
	void UpdateLevel();

protected:


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartBasicAttack(const FInputActionValue& Value);

	void TakeDamage(float DamageAmount);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float playerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks");
	bool BasicAttackUsed;

	UPROPERTY()
	UUserWidget* HealthBarWidget1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
			

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	class UUPlayerHealthBar* UPlayerHealthBar;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

