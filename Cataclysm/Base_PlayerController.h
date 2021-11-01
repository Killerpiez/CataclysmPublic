// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cataclysm/Items/Base_Item.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "Base_PlayerController.generated.h"

class ACataclysmCharacter;

UCLASS()
class CATACLYSM_API ABase_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABase_PlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void CallGameStateSpawnActor(FInventoryData ItemData, AActor* OwningActor);

protected:
	
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:

	UPROPERTY()
		TSubclassOf<class UUserWidget> InventoryClass;

	UPROPERTY()
		TSubclassOf<class UUserWidget> ItemClass;

	UPROPERTY()
		class UWidget_Inventory* Inventory;

	UFUNCTION()
		void ToggleInventory();

	UFUNCTION(Server, reliable, WithValidation)
		void Server_SpawnActor(FInventoryData ItemData, AActor* OwningActor);
};