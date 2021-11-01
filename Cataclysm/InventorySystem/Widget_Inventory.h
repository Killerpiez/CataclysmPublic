// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Widget_Inventory.generated.h"

class UWidget_InventoryGrid;
class UInventoryComponent;

UCLASS()
class CATACLYSM_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void SetInventoryComponentReference(UInventoryComponent* InventoryComponentReference);

	UFUNCTION()
		void SetItemClassReference(TSubclassOf<class UUserWidget> WidgetItemClass);

	UFUNCTION(Blueprintpure)
		UInventoryComponent* GetInventoryComponent();

protected:

	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
		class UWidget_InventoryGrid* SomethingTheFuckingEditorWillAcceptAsAFuckingName;

	UPROPERTY(meta = (BindWidget))
		class UBorder* BackgroundBorder;

	UPROPERTY(meta = (BindWidget))
		class UBackgroundBlur* Blur;

	UPROPERTY(meta = (BindWidget))
		class USizeBox* GridBorder;

	UPROPERTY()
		UInventoryComponent* InventoryComponent;

	UPROPERTY()
		bool HasBeenCreated = false;
};