// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Cataclysm/InventorySystem/InventoryComponent.h"
#include "Cataclysm/InventorySystem/Widget_Item.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "SlateMaterialBrush.h"
#include "Widget_InventoryGrid.h"

bool UWidget_InventoryGrid::Initialize()
{
	bool Success = Super::Initialize();

	return Success;
}

void UWidget_InventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_InventoryGrid::SetInventoryComponentReference(UInventoryComponent* InventoryComponentReference)
{
	InventoryComponent = InventoryComponentReference;

	InventoryComponent->OnInventoryChanged.AddDynamic(this, &UWidget_InventoryGrid::Refresh);
}

void UWidget_InventoryGrid::SetWidgetItemClass(TSubclassOf<class UUserWidget> Class)
{
	WidgetItemClass = Class;
}

void UWidget_InventoryGrid::CreateGridLineList()
{
	FGridLines LocalGridLine;

	if (InventoryComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget_InventoryGrid: Inventory Component is NULL"));
		return;
	}

	// Create vertical line list
	for (int32 i = 0; i <= InventoryComponent->GetNumRows(); i++)
	{
		LocalGridLine.StartPoint.X = 0.0f;
		LocalGridLine.StartPoint.Y = i * InventoryComponent->GetTileSize();
		LocalGridLine.EndPoint.X = InventoryComponent->GetTileSize() * InventoryComponent->GetNumColumns();
		LocalGridLine.EndPoint.Y = i * InventoryComponent->GetTileSize();

		GridLineList.Add(LocalGridLine);
	}

	// create horizontal line list
	for (int32 i = 0; i <= InventoryComponent->GetNumColumns(); i++)
	{
		LocalGridLine.StartPoint.X = i * InventoryComponent->GetTileSize();
		LocalGridLine.StartPoint.Y = 0.0f;
		LocalGridLine.EndPoint.X = i * InventoryComponent->GetTileSize();
		LocalGridLine.EndPoint.Y = InventoryComponent->GetTileSize() * InventoryComponent->GetNumRows();

		GridLineList.Add(LocalGridLine);
	}
}

void UWidget_InventoryGrid::Refresh()
{
	TArray<FInventoryData> DrawImageList = InventoryComponent->GetInventoryList();

	// This list is the list of ONLY top left indices so I know where to draw each panel
	TArray<int32> IndexList = InventoryComponent->GetTopLeftInventoryIndices();

	CanvasGrid->ClearChildren();

	for (int32 i = 0; i < IndexList.Num(); i++)
	{
		WidgetItem = CreateWidget<UWidget_Item>(this, WidgetItemClass);
		WidgetItem->CallItemRemove.AddDynamic(this, &UWidget_InventoryGrid::CallInventoryComponentRemoveItem);
		WidgetItem->CallItemSplit.AddDynamic(this, &UWidget_InventoryGrid::HandleItemSplit);
		WidgetItem->SetItemObjectData(DrawImageList[IndexList[i]]);
		WidgetItem->SetTopLeftCorner(IndexList[i]);

		auto LocalNewPanel = CanvasGrid->AddChild(WidgetItem);

		BroadcastItemWidgetCreation(WidgetItem, DrawImageList[IndexList[i]]);

		UCanvasPanelSlot* LocalPanelSlot = Cast<UCanvasPanelSlot>(LocalNewPanel);
		LocalPanelSlot->SetAutoSize(true);
		FVector2D ItemPositionVector = FVector2D(InventoryComponent->IndexToTile(IndexList[i]).X, InventoryComponent->IndexToTile(IndexList[i]).Y) * InventoryComponent->GetTileSize();

		// I have no idea why its shifted off by a few pixels... this corrects it for now. Temp fix until I discover the real issue 
		ItemPositionVector.X = ItemPositionVector.X - 2.5f;

		LocalPanelSlot->SetPosition(ItemPositionVector);
	}
}

UInventoryComponent* UWidget_InventoryGrid::GetInventoryComponent()
{
	return InventoryComponent;
}

void UWidget_InventoryGrid::CallInventoryComponentRemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft)
{
	InventoryComponent->RemoveItem(SizeX, SizeY, TopLeft);
}

void UWidget_InventoryGrid::BroadcastItemWidgetCreation(UWidget_Item* WidgetToRefresh, FInventoryData ItemData)
{
	// BP_InventoryGrid is listening
	CreateItemWidgetBrush.Broadcast(WidgetToRefresh, ItemData);
}

void UWidget_InventoryGrid::HandleItemSplit(UWidget_Item* ItemWidget, FInventoryData ItemData)
{
	FInventoryData LocalItemData = ItemData;

	LocalItemData.CurrentStack = ItemData.CurrentStack / 2;
	ItemData.CurrentStack = ItemData.CurrentStack - LocalItemData.CurrentStack;

	if (InventoryComponent->TryAddItem(ItemData, true))
	{
		// We AddItem here assuming its good because the item already exists in this spot
		InventoryComponent->AddItem(ItemWidget->GetTopLeftCorner(), LocalItemData, false);
		Refresh();
	}
}