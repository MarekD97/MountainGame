// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "ClothItem.generated.h"
UENUM()
enum EClothType
{
	Headwear,
	Torso,
	Legs,
	Hands,
	Boots,
	Backpack
};

UCLASS()
class MOUNTAINEXPEDITION_API UClothItem : public UItem
{
	GENERATED_BODY()

public:
	UClothItem();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class USkeletalMesh* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TEnumAsByte<EClothType> ClothType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	uint8 bEquipped:1;

	virtual void Use(APlayerCharacter* Character) override;

	
};
