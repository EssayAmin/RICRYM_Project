// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject3GameMode.h"
#include "MyProject3Character.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Components/Button.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ConstructorHelpers.h"

AMyProject3GameMode::AMyProject3GameMode()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/userSelectionWidget"));
    if (WidgetClassFinder.Succeeded())
    {
        CharacterSelectionWidgetClass = WidgetClassFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetClassFinder(TEXT("/Game/UI/HealthBar2"));
    if (HealthWidgetClassFinder.Succeeded())
    {
        HealthBarWidgetClass2 = HealthWidgetClassFinder.Class;
    }

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    static ConstructorHelpers::FClassFinder<AMyProject3Character> PlayerCharacterBP(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerCharacterBP.Succeeded())
    {
        PlayerCharacterClass = PlayerCharacterBP.Class;
    }

}

bool AMyProject3GameMode::bIsWidgetAdded = false;

void AMyProject3GameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!bIsWidgetAdded) // Only show the widget if it's not already added
    {
        ShowCharacterSelectionMenu();  // Show the character selection menu when the game starts


        UButton* StartGameButton = Cast<UButton>(CharacterSelectionWidgetInstance->GetWidgetFromName(TEXT("Button_31")));
        UButton* ExitButton = Cast<UButton>(CharacterSelectionWidgetInstance->GetWidgetFromName(TEXT("Button")));

        // Bind button events
        if (StartGameButton)
        {
            StartGameButton->OnClicked.AddDynamic(this, &AMyProject3GameMode::OnStartGameClicked);
        }
        if (ExitButton)
        {
            ExitButton->OnClicked.AddDynamic(this, &AMyProject3GameMode::OnExitClicked);
        }
    }

}


void AMyProject3GameMode::ShowCharacterSelectionMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("casting "));
    if (CharacterSelectionWidgetClass)
    {
        // Create the widget instance
        

        if (!CharacterSelectionWidgetInstance)
        {
            CharacterSelectionWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CharacterSelectionWidgetClass);
            // Add the widget to the viewport
            CharacterSelectionWidgetInstance->AddToViewport();

            // Set input mode to UI only (this will disable the game controls and show the mouse cursor)
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(CharacterSelectionWidgetInstance->GetCachedWidget());
            GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
            GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

            UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport!"));

            bIsWidgetAdded = true;
            
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CharacterSelectionWidgetClass is not set!"));
        }
    }
}

void AMyProject3GameMode::OnStartGameClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Game button clicked!"));
    // Logic to start the game, e.g., open a level

    if (CharacterSelectionWidgetInstance)
    {
        CharacterSelectionWidgetInstance->RemoveFromViewport();  // Removes the widget
        UE_LOG(LogTemp, Warning, TEXT("Widget Remove From viewport!"));
    }
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);  // Get Player 1's controller
    if (PlayerController)
    {
        // Set input mode to game-only mode
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);  // Switch to game input mode
        PlayerController->bShowMouseCursor = false; // Hide mouse cursor if you don't want it visible

        TArray<AActor*> PlayerStarts;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

        APlayerStart* PlayerStart2 = Cast<APlayerStart>(PlayerStarts[1]);
        if (PlayerStart2)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found PlayerStart2 at: %s"), *PlayerStart2->GetActorLocation().ToString());
        }

        APlayerController* PlayerController2 = UGameplayStatics::CreatePlayer(GetWorld(), 1);
        if (PlayerController2)
        {
            APawn* PlayerPawn2 = PlayerController2->GetPawn();
            PlayerController2->Possess(Cast<APawn>(PlayerPawn2));

            if (PlayerPawn2)
            {
                UE_LOG(LogTemp, Warning, TEXT("Player 2's Pawn: %s"), *PlayerPawn2->GetName());

                AMyProject3Character* PlayerCharacter2 = Cast<AMyProject3Character>(PlayerPawn2);

                if (PlayerCharacter2)
                {
                    PlayerCharacter2->SetPlayerRole(EPlayerRole::Mage); // Assign "Mage" role to player 2
                    UE_LOG(LogTemp, Warning, TEXT("Assigning Mage!"));
                }

                HealthBarWidget2 = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidgetClass2);
                if (HealthBarWidget2)
                {
                    HealthBarWidget2->AddToViewport();
                }

            }
            
        }

    //UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));

    
    }

    
}

void AMyProject3GameMode::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit button clicked!"));
    // Quit the application
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}