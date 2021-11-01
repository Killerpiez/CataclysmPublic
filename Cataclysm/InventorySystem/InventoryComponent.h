// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cataclysm/Items/Base_Item.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UMaterialInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallRefresh);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATACLYSM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	UPROPERTY()
		FCallRefresh OnInventoryChanged;

	UFUNCTION()
		int32 GetNumRows();

	UFUNCTION()
		int32 GetNumColumns();

	UFUNCTION()
		float GetTileSize();

	UFUNCTION(Blueprintpure)
		TArray<int32> GetTopLeftInventoryIndices();

	UFUNCTION(Blueprintpure)
		TArray<FInventoryData> GetInventoryList();

	UFUNCTION(Blueprintpure)
		int32 GetLastItemTopLeft();

	UFUNCTION(Blueprintpure)
		bool CompareFInventoryData(FInventoryData Data1, FInventoryData Data2);

	UFUNCTION(BlueprintCallable)
		bool TryAddItem(FInventoryData ItemData, bool IgnoreStack);

	// Tries to add the item back to its last spot before adding it randomly in the inventory
	UFUNCTION(BlueprintCallable)
		bool TryReturnItem(FInventoryData ItemData, int32 LastTopLeftCorner);

	UFUNCTION(BlueprintCallable)
		bool IsStackAvailable(int32 IndexToCheck, FInventoryData ItemData);

	// Tries to do a partial stack then add the remaining to the inventory (This assumes IsStackAvailable has already been called)
	UFUNCTION(BlueprintCallable)
		bool TryPartialStack(int32 IndexToCheck, UPARAM(ref) FInventoryData& ItemData);

	UFUNCTION(BlueprintCallable)
		bool FindValidTopLeft(int32 TopLeftIndex, FInventoryData ItemData);

	UFUNCTION(BlueprintCallable)
		void AddItem(int32 TopLeftIndex, FInventoryData ItemData, bool Stacking);

	UFUNCTION(BlueprintCallable)
		void RotateItem(FInventoryData& NewItemDataOUT);

	UFUNCTION(Blueprintpure)
		FVector2D IndexToTile(int32 Index);

	UFUNCTION(Blueprintpure)
		int32 TileToIndex(FVector2D Tile);

	UFUNCTION()
		void RemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InventoryGrid, meta = (AllowPrivateAccess = "true"))
		int32 NumRows = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InventoryGrid, meta = (AllowPrivateAccess = "true"))
		int32 NumColumns = 5;

	UPROPERTY(BlueprintReadOnly, Category = IneventoryGrid, meta = (AllowPrivateAccess = "true"))
		float TileSize = 50.0f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 LastItemTopLeft = 0;

	UPROPERTY()
		TArray<FInventoryData> InventoryList;

	UPROPERTY()
		TArray<int32> TopLeftIndexList;

	UFUNCTION()
		bool IsTileValid(FVector2D Tile);

	UFUNCTION()
		bool IsTileOpen(FVector2D Tile);
};