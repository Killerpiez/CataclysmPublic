// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cataclysm/Items/Base_Item.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CataclysmGameState.generated.h"

/**
 * 
 */
UCLASS()
class CATACLYSM_API ACataclysmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void SpawnItemFromActor(FInventoryData ItemData, AActor* ActorToSpawnFrom, bool Clamping);

protected:

private:
};
