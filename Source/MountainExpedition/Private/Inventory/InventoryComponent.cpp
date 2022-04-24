// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Inventory/Item.h"
#include "Inventory/ClothItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20.0f;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bDefaultEnabled)
	{
		for (auto& Item : DefaultItems)
		{
			AddItem(Item);
		}
	}
	
}

bool UInventoryComponent::AddItem(UItem* Item)
{
	float TotalWeight = 0;
	for (int i = 0; i < Items.Num(); i++)
	{
		TotalWeight += Items[i]->Weight;
	}

	if (Item && TotalWeight + Item->Weight <= Capacity )
	{
		Item->OwningInventory = this;
		Item->World = GetWorld();
		Items.Add(Item);

		// Update UI

		OnInventoryUpdated.Broadcast();

		return true;
	}
	
	return false;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	if (Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::EquipItem(UClothItem* Item)
{
	if (Item)
	{
		for (UClothItem* EquippedItem : EquippedItems)
		{
			if(EquippedItem->ClothType == Item->ClothType)
			{
				UnEquipItem(EquippedItem);
				break;
			}
		}
		Item->bEquipped = true;
		EquippedItems.Add(Item);
		RemoveItem(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::UnEquipItem(UClothItem* Item)
{
	if (Item)
	{
		Item->bEquipped = false;
		EquippedItems.RemoveSingle(Item);
		AddItem(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}


