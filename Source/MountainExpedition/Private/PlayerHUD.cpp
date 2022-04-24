// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "MainGameMode.h"

APlayerHUD::APlayerHUD()
{
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	switch (Cast<AMainGameMode>(GetWorld()->GetAuthGameMode())->CurrentGameMode)
	{
	case EGameMode::MainMenu:
	{
		ShowMainMenu();
		break;
	}
	case EGameMode::PauseMenu:
	{
		break;
	}
	case EGameMode::Gameplay:
	{
		break;
	}
	}
}

void APlayerHUD::ShowMainMenu()
{
	if (MainMenuWidget != nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		MainMenuWidget->AddToViewport();
		Cast<AMainGameMode>(GetWorld()->GetAuthGameMode())->SetGameMode(EGameMode::MainMenu);

		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(MainMenuWidget->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void APlayerHUD::ShowSettingsMenu()
{
	MainMenuWidget->Destruct();
	if (SettingsMenuWidget != nullptr)
	{
		SettingsMenuWidget->AddToViewport();
	}
}
void APlayerHUD::ShowGameplayHUD()
{
	MainMenuWidget->Destruct();
	if (GameplayWidget != nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		GameplayWidget->AddToViewport();
		Cast<AMainGameMode>(GetWorld()->GetAuthGameMode())->SetGameMode(EGameMode::Gameplay);

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}

}

void APlayerHUD::ShowPauseMenu()
{
	GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Yellow, TEXT("ShowPauseMenu"));
}

void APlayerHUD::Inventory_Implementation(bool bEnabled)
{
	if (InventoryWidget)
	{
		if (bEnabled)
		{
			InventoryWidget->AddToViewport();
		}
		else
		{
			InventoryWidget->Destruct();
		}
	}
}