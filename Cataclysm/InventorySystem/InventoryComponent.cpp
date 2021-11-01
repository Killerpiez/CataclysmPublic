// Fill out your copyright notice in the Description page of Project Settings.

#include "Cataclysm/Base_PlayerController.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryList.SetNum(NumRows * NumColumns);
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::TryAddItem(FInventoryData ItemData, bool IgnoreStack)
{
	if (ItemData.Stackable && !IgnoreStack)
	{
		for (int32 i = 0; i < InventoryList.Num(); i++)
		{
			if (IsStackAvailable(i, ItemData) && (InventoryList[i].CurrentStack + ItemData.CurrentStack < InventoryList[i].StackSize))
			{
				AddItem(i, ItemData, true);
				return true;
			}
		}
	}

	for (int32 i = 0; i < InventoryList.Num(); i++)
	{
		if (FindValidTopLeft(i, ItemData))
		{
			AddItem(i, ItemData, false);
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::TryReturnItem(FInventoryData ItemData, int32 LastTopLeftCorner)
{
	if (IsStackAvailable(LastTopLeftCorner, ItemData))
	{
		AddItem(LastTopLeftCorner, ItemData, true);
		return true;
	}

	if (FindValidTopLeft(LastTopLeftCorner, ItemData))
	{
		AddItem(LastTopLeftCorner, ItemData, false);
		return true;
	}

	return false;
}

void UInventoryComponent::AddItem(int32 TopLeftIndex, FInventoryData ItemData, bool Stacking)
{
	FVector2D StartingTile = IndexToTile(TopLeftIndex);

	TopLeftIndexList.AddUnique(TopLeftIndex);

	for (int32 i = StartingTile.X; i < StartingTile.X + ItemData.SizeX; i++)
	{
		for (int32 j = StartingTile.Y; j < StartingTile.Y + ItemData.SizeY; j++)
		{
			
			if (Stacking)
			{
				InventoryList[TileToIndex(FVector2D(i, j))].CurrentStack = InventoryList[TileToIndex(FVector2D(i, j))].CurrentStack + ItemData.CurrentStack;
			}
			else
			{		
				InventoryList[TileToIndex(FVector2D(i, j))] = ItemData;
			}

			ItemData.UseDisplayMesh = false;
		}
	}
}

bool UInventoryComponent::FindValidTopLeft(int32 TopLeftIndex, FInventoryData ItemData)
{
	FVector2D StartingTile = IndexToTile(TopLeftIndex);

	for (int32 i = StartingTile.X; i < StartingTile.X + ItemData.SizeX; i++)
	{
		for (int32 j = StartingTile.Y; j < StartingTile.Y + ItemData.SizeY; j++)
		{
			if (!IsTileValid(FVector2D(i, j)))
			{
				return false;
			}

			if (!IsTileOpen(FVector2D(i, j)))
			{
				return false;
			}
		}
	}

	return true;
}

bool UInventoryComponent::IsTileValid(FVector2D Tile)
{
	if (Tile.X >= 0 && Tile.X < NumColumns && Tile.Y >= 0 && Tile.Y < NumRows)
	{
		return true;
	}

	return false;
}

bool UInventoryComponent::IsTileOpen(FVector2D Tile)
{
	return (InventoryList[TileToIndex(Tile)].ItemClass == nullptr);
}

bool UInventoryComponent::IsStackAvailable(int32 IndexToCheck, FInventoryData ItemData)
{
	if (0 <= IndexToCheck && IndexToCheck < InventoryList.Num())
	{
		if (CompareFInventoryData(InventoryList[IndexToCheck], ItemData))
		{
			return InventoryList[IndexToCheck].CurrentStack < InventoryList[IndexToCheck].StackSize;
		}
	}
	return false;
}

// IndexToCheck gets the current amound sitting at that location, ItemData is the changed stack that should essentially split.
// returns true if there is possible remaining stacks
// returns false if we should just do a regular add
bool UInventoryComponent::TryPartialStack(int32 IndexToCheck, UPARAM(ref) FInventoryData& ItemData)
{
	int32 LocalTotal = InventoryList[IndexToCheck].CurrentStack + ItemData.CurrentStack;
	int32 LocalRemaining = 0;

	if (InventoryList[IndexToCheck].StackSize < LocalTotal)
	{
		LocalRemaining = LocalTotal - InventoryList[IndexToCheck].StackSize;

		if (LocalRemaining > 0)
		{
			ItemData.CurrentStack = LocalRemaining;
		}
		else
		{
			ItemData.CurrentStack = 0;
		}

		InventoryList[IndexToCheck].CurrentStack = InventoryList[IndexToCheck].StackSize;
		return true;
	}

	return false;
}

// If I add anything to FInventoryData, I need to comeback and reevaluate this function
bool UInventoryComponent::CompareFInventoryData(FInventoryData Data1, FInventoryData Data2)
{
	if (
		Data1.Icon == Data2.Icon &&
		Data1.IconRotated == Data2.IconRotated &&
		Data1.ItemClass == Data2.ItemClass &&
		Data1.SizeX == Data2.SizeX &&
		Data1.SizeY == Data2.SizeY &&
		Data1.Stackable == Data2.Stackable &&
		Data1.StackSize == Data2.StackSize
		)
	{
		return true;
	}

	return false;
}

FVector2D UInventoryComponent::IndexToTile(int32 Index)
{
	FVector2D Tile;

	Tile.X = Index % NumColumns;
	Tile.Y = Index / NumColumns;

	return Tile;
}

int32 UInventoryComponent::TileToIndex(FVector2D Tile)
{
	int32 Index = Tile.X + (Tile.Y * NumColumns);
	return Index;
}

int32 UInventoryComponent::GetNumRows()
{
	return NumRows;
}

int32 UInventoryComponent::GetNumColumns()
{
	return NumColumns;
}

float UInventoryComponent::GetTileSize()
{
	return TileSize;
}

TArray<int32> UInventoryComponent::GetTopLeftInventoryIndices()
{
	return TopLeftIndexList;
}

TArray<FInventoryData> UInventoryComponent::GetInventoryList()
{
	return InventoryList;
}

int32 UInventoryComponent::GetLastItemTopLeft()
{
	return LastItemTopLeft;
}

void UInventoryComponent::RemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft)
{
	LastItemTopLeft = TopLeft;
	TopLeftIndexList.RemoveAt(TopLeftIndexList.Find(TopLeft));

	FVector2D LocalTile = IndexToTile(TopLeft);

	for (int32 i = LocalTile.X; i < LocalTile.X + SizeX; i++)
	{
		for (int32 j = LocalTile.Y; j < LocalTile.Y + SizeY; j++)
		{
			InventoryList[TileToIndex(FVector2D(i, j))] = FInventoryData();
		}
	}
}

void UInventoryComponent::RotateItem(FInventoryData& NewItemDataOUT)
{
	if (NewItemDataOUT.Rotated)
	{
		NewItemDataOUT.Rotated = false;
	}
	else
	{
		NewItemDataOUT.Rotated = true;
	}
	uint8 NewX = NewItemDataOUT.SizeY;
	uint8 NewY = NewItemDataOUT.SizeX;

	NewItemDataOUT.SizeX = NewX;
	NewItemDataOUT.SizeY = NewY;
}