// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DrinkItem.h"
#include "Inventory/InventoryComponent.h"
#include "PlayerCharacter.h"

UDrinkItem::UDrinkItem()
{
	Irrigation = 1.0;
}

void UDrinkItem::Use(class APlayerCharacter* Character)
{
	if (Character && Character->Thirst < 1.0f)
	{
		Character->Thirst += Irrigation;
		Weight -= Irrigation;

		if (Character->Thirst >= 1.0f)
		{
			Character->Thirst = 1.0f;
		}

		if (Weight <= 0.0f && OwningInventory)
		{
			OwningInventory->RemoveItem(this);
		}
	}
}