// Fill out your copyright notice in the Description page of Project Settings.


#include "Cataclysm/InventorySystem/Widget_Item.h"

void UWidget_Item::BroadcastRemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft)
{
	// Widget_InventoryGrid is listening
	CallItemRemove.Broadcast(SizeX, SizeY, TopLeft);
}

void UWidget_Item::BroadcastItemSplit(UWidget_Item* ItemWidget, FInventoryData ItemData)
{
	CallItemSplit.Broadcast(ItemWidget, ItemData);
}

void UWidget_Item::SetTopLeftCorner(int32 TopLeftCornerIN)
{
	TopLeftCorner = TopLeftCornerIN;
}

int32 UWidget_Item::GetTopLeftCorner()
{
	return TopLeftCorner;
}

void UWidget_Item::SetItemObjectData(FInventoryData ItemObjectDataIN)
{
	ItemObjectData = ItemObjectDataIN;
}

FInventoryData UWidget_Item::GetItemObjectData()
{
	return ItemObjectData;
}
