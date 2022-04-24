// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Item.h"
#include "Inventory/InventoryComponent.h"

UItem::UItem()
{
	Weight = 1.0f;

	ItemDisplayName = FText::FromString("Item");
	UseActionName = FText::FromString("Use");
}

void UItem::Use(APlayerCharacter* Character)
{
	if (Character && OwningInventory)
	{
		OwningInventory->RemoveItem(this);
	}
}
