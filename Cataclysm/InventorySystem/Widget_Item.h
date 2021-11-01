// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cataclysm/Items/Base_Item.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCallItemRemove, uint8, SizeX, uint8, SizeY, int32, TopLeftCorner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCallItemSplit, UWidget_Item*, ItemWidget, FInventoryData, ItemData);

UCLASS()
class CATACLYSM_API UWidget_Item : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class USizeBox* ImageSize;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UImage* IconImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UTextBlock* StackedAmount;

	UPROPERTY(BlueprintReadWrite)
		UMaterialInterface* IconImageToSet;

	UPROPERTY(BlueprintReadWrite)
		FVector2D ImageSizeToSet;

	UPROPERTY(BlueprintReadWrite)
		FString CurrentStackText;

	UPROPERTY()
		FCallItemRemove CallItemRemove;

	UPROPERTY()
		FCallItemSplit CallItemSplit;
	
	UFUNCTION()
		void SetTopLeftCorner(int32 TopLeftCornerIN);

	UFUNCTION()
		int32 GetTopLeftCorner();

	UFUNCTION()
		void SetItemObjectData(FInventoryData ItemObjectDataIN);

	UFUNCTION(Blueprintpure)
		FInventoryData GetItemObjectData();

	UFUNCTION(BlueprintCallable)
		void BroadcastRemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft);

	UFUNCTION(BlueprintCallable)
		void BroadcastItemSplit(UWidget_Item* ItemWidget, FInventoryData ItemData);

protected:

	UPROPERTY(BlueprintReadOnly)
		int32 TopLeftCorner;

private:

	UPROPERTY()
		FInventoryData ItemObjectData;
};
