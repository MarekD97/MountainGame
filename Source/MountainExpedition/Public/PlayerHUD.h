// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

UCLASS()
class MOUNTAINEXPEDITION_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	APlayerHUD();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Widget)
		class UUserWidget* MainMenuWidget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Widget)
		class UUserWidget* SettingsMenuWidget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Widget)
		class UUserWidget* GameplayWidget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Widget)
		class UUserWidget* InventoryWidget;
	
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void ShowSettingsMenu();

	UFUNCTION(BlueprintCallable)
	void ShowGameplayHUD();

	UFUNCTION(BlueprintCallable)
		void ShowPauseMenu();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Inventory(bool bEnabled);

	
};
