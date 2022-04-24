// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINEXPEDITION_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APlayerHUD* PlayerHUD;

	bool bIsInventoryOpened;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
public:
	AMainPlayerController();

	UFUNCTION()
	void Inventory();

	UFUNCTION(BlueprintImplementableEvent)
	void Die();
};
