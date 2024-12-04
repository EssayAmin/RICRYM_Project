// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyProject3Character.h"
#include "Kismet/GameplayStatics.h"
#include "DamageDummyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API ADamageDummyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	
};
