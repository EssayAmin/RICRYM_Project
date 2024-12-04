// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageDummyAIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ADamageDummyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); 

    if (PlayerCharacter)
    {
        MoveToActor(PlayerCharacter, 5.0f); 
    }
}