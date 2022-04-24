// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUNTAINEXPEDITION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool AddItem(class UItem* Item);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(class UItem* Item);

	UFUNCTION(BlueprintCallable)
	bool EquipItem(class UClothItem* Item);
	UFUNCTION(BlueprintCallable)
	bool UnEquipItem(class UClothItem* Item);

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class UItem*> DefaultItems;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Items")
	TArray<class UItem*> Items;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Equipment")
	TArray<class UClothItem*> EquippedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 Capacity;

	UPROPERTY(EditDefaultsOnly)
	uint8 bDefaultEnabled : 1;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
		
};
