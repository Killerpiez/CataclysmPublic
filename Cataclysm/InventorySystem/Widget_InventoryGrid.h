// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cataclysm/Items/Base_Item.h"
#include "Widget_InventoryGrid.generated.h"

class UInventoryComponent;
class ABase_Item;
class UWidget_Item;

USTRUCT(BlueprintType)
struct FGridLines
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = GridLines, meta = (AllowPrivateAccess = "true"))
		FVector2D StartPoint;

	UPROPERTY(BlueprintReadOnly, Category = GridLines, meta = (AllowPrivateAccess = "true"))
		FVector2D EndPoint;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateItemWidgetBrush, UWidget_Item*, WidgetCreated, FInventoryData, ItemData);

UCLASS()
class CATACLYSM_API UWidget_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Create ItemWidget Brush")
		FCreateItemWidgetBrush CreateItemWidgetBrush;

	UFUNCTION()
		void SetInventoryComponentReference(UInventoryComponent* InventoryComponentReference);

	UFUNCTION()
		void SetWidgetItemClass(TSubclassOf<class UUserWidget> Class);

	UFUNCTION()
		void CreateGridLineList();

	UFUNCTION(BlueprintCallable)
		void Refresh();

	UFUNCTION(Blueprintpure)
		UInventoryComponent* GetInventoryComponent();

	UFUNCTION()
		void BroadcastItemWidgetCreation(UWidget_Item* WidgetToRefresh, FInventoryData ItemData);

protected:

	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
		class UBorder* CenteredBorder;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasGrid;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = GridLines, meta = (AllowPrivateAccess = "true"))
		class UCanvasPanel* LocationPanel;

	UPROPERTY()
		class UWidget_Item* WidgetItem;

	UPROPERTY()
		TSubclassOf<class UUserWidget> WidgetItemClass;

	UPROPERTY()
		UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadOnly, Category = GridLines, meta = (AllowPrivateAccess = "true"))
		TArray<FGridLines> GridLineList;

	UFUNCTION()
		void CallInventoryComponentRemoveItem(uint8 SizeX, uint8 SizeY, int32 TopLeft);

	UFUNCTION()
		void HandleItemSplit(UWidget_Item* ItemWidget, FInventoryData ItemData);
};