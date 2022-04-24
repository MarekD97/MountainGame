// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ClothItem.h"

#include "PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"

UClothItem::UClothItem()
{
	bEquipped = false;
}

void UClothItem::Use(APlayerCharacter* Character)
{
	if(Character)
	{
		if(bEquipped)
		{
			Character->InventoryComponent->UnEquipItem(this);
		}
		else
		{
			Character->InventoryComponent->EquipItem(this);
		}
		
	}
}
