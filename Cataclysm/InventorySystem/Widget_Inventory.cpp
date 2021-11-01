// Fill out your copyright notice in the Description page of Project Settings.


#include "Cataclysm/InventorySystem/InventoryComponent.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Widget_InventoryGrid.h"
#include "Widget_Inventory.h"

bool UWidget_Inventory::Initialize()
{
	bool Success = Super::Initialize();

	return Success;
}

void UWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponent != nullptr)
	{
		if (!HasBeenCreated)
		{
			GridBorder->SetWidthOverride(InventoryComponent->GetNumColumns() * InventoryComponent->GetTileSize());
			GridBorder->SetHeightOverride(InventoryComponent->GetNumRows() * InventoryComponent->GetTileSize());

			SomethingTheFuckingEditorWillAcceptAsAFuckingName->CreateGridLineList();

			HasBeenCreated = true;
		}

		SomethingTheFuckingEditorWillAcceptAsAFuckingName->Refresh();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget_Inventory: Inventory Component is NULL"));
	}
}

void UWidget_Inventory::SetInventoryComponentReference(UInventoryComponent* InventoryComponentReference)
{
	InventoryComponent = InventoryComponentReference;
	SomethingTheFuckingEditorWillAcceptAsAFuckingName->SetInventoryComponentReference(InventoryComponent);
}

void UWidget_Inventory::SetItemClassReference(TSubclassOf<class UUserWidget> WidgetItemClass)
{
	SomethingTheFuckingEditorWillAcceptAsAFuckingName->SetWidgetItemClass(WidgetItemClass);
}

UInventoryComponent* UWidget_Inventory::GetInventoryComponent()
{
	return InventoryComponent;
}
