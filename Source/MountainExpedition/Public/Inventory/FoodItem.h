// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINEXPEDITION_API UFoodItem : public UItem
{
	GENERATED_BODY()

public:
	UFoodItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float Saturation;

	virtual void Use(class APlayerCharacter* Character) override;
	
};
