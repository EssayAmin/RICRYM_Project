// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProject3Character.h"
#include "GameFramework/PlayerStart.h"
#include "MyProject3GameMode.generated.h"


class AMyProject3Character;

UCLASS(minimalapi)
class AMyProject3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProject3GameMode();

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> CharacterSelectionWidgetClass;

    // Store the instance of the widget
    UPROPERTY()
    UUserWidget* CharacterSelectionWidgetInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FString LevelName;

    // Function to show the widget
    void ShowCharacterSelectionMenu();

    UFUNCTION()
    void OnStartGameClicked();

    UFUNCTION()
    void OnExitClicked();

    static bool bIsWidgetAdded;

    UPROPERTY()
    UUserWidget* HealthBarWidget2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> HealthBarWidgetClass2;

protected:


    UPROPERTY(EditDefaultsOnly, Category = "Player")
    TSubclassOf<AMyProject3Character> PlayerCharacterClass;

};



