// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PlayerHUD.h"


AMainPlayerController::AMainPlayerController()
{
	bIsInventoryOpened = false;
}


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(GetHUD());
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AMainPlayerController::Inventory);
}
void AMainPlayerController::Inventory()
{
	if (PlayerHUD)
	{
		if (!bIsInventoryOpened)
		{
			PlayerHUD->Inventory(true);
			bIsInventoryOpened = bShowMouseCursor = true;
			SetInputMode(FInputModeGameAndUI());
			// UWidgetBlueprintLibrary::SetInputMode_GameAndUI(this, nullptr, true, false);
		}
		else
		{
			PlayerHUD->Inventory(false);
			bIsInventoryOpened = bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			// UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		}
	}
}