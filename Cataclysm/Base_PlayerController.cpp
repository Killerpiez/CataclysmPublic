// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_PlayerController.h"

#include "CataclysmCharacter.h"
#include "CataclysmGameState.h"
#include "Components/DecalComponent.h"
#include "InventorySystem/Widget_Inventory.h"
#include "Net/UnrealNetwork.h"

ABase_PlayerController::ABase_PlayerController(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> InventoryBPClass(TEXT("/Game/InventorySystem/BP_Inventory"));
	if (InventoryBPClass.Class == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController: InvetoryBPClass is NULL"));
		return;
	}

	InventoryClass = InventoryBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ItemBPClass(TEXT("/Game/InventorySystem/BP_Item"));
	if (ItemBPClass.Class == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController: ItemBPClass is NULL"));
		return;
	}

	ItemClass = ItemBPClass.Class;
}

void ABase_PlayerController::BeginPlay()
{
	// Get a reference to the player
	auto PlayerReference = Cast<ACataclysmCharacter>(GetPawn());

	if (IsLocalController())
	{
		Inventory = CreateWidget<UWidget_Inventory>(this, InventoryClass);

		if (Inventory == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController: Inventory is NULL"));
			return;
		}

		Inventory->SetInventoryComponentReference(PlayerReference->GetInventoryComponent());
		Inventory->SetItemClassReference(ItemClass);
	}
}

void ABase_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ABase_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ABase_PlayerController::ToggleInventory);
}

void ABase_PlayerController::ToggleInventory()
{
	if (Inventory->IsVisible())
	{
		Inventory->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);
		Inventory->RemoveFromParent();
	}
	else
	{
		Inventory->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeData;
		SetInputMode(InputModeData);
		Inventory->AddToViewport();
	}
}

void ABase_PlayerController::CallGameStateSpawnActor(FInventoryData ItemData, AActor* OwningActor)
{
	Server_SpawnActor(ItemData, OwningActor);
}

void ABase_PlayerController::Server_SpawnActor_Implementation(FInventoryData ItemData, AActor* OwningActor)
{
	// Actual item drop
	auto GameStateReference = Cast<ACataclysmGameState>(GetWorld()->GetGameState());

	GameStateReference->SpawnItemFromActor(ItemData, OwningActor, true);
}

bool ABase_PlayerController::Server_SpawnActor_Validate(FInventoryData ItemData, AActor* OwningActor)
{
	if (ItemData.ItemClass != nullptr)
	{
		return true;
	}

	return false;
}