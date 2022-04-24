// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/FoodItem.h"
#include "Inventory/InventoryComponent.h"
#include "PlayerCharacter.h"

UFoodItem::UFoodItem()
{
	Saturation = 1.0f;
}

void UFoodItem::Use(class APlayerCharacter* Character)
{
	if (Character && Character->Hunger < 1.0f)
	{
		Character->Hunger += Saturation;
		if (Character->Hunger >= 1.0f)
		{
			Character->Hunger = 1.0f;
		}

		if (OwningInventory)
		{
			OwningInventory->RemoveItem(this);
		}
	}
}