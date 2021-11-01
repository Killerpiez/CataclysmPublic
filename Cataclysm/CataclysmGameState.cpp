// Fill out your copyright notice in the Description page of Project Settings.


#include "CataclysmGameState.h"

void ACataclysmGameState::SpawnItemFromActor(FInventoryData ItemData, AActor* ActorToSpawnFrom, bool Clamping)
{
	FVector LocalSpawnLocation;
	FHitResult LocalHitLocation;
	AActor* SpawnedActor;

	LocalSpawnLocation = ActorToSpawnFrom->GetActorLocation() + (ActorToSpawnFrom->GetActorForwardVector() * 150.0f);

	if (Clamping)
	{
		if (GetWorld()->LineTraceSingleByChannel(LocalHitLocation, LocalSpawnLocation, LocalSpawnLocation - FVector(0.0f, 0.0f, 10000.0f), ECC_Visibility))
		{
			LocalSpawnLocation = LocalHitLocation.Location;

			UE_LOG(LogTemp, Warning, TEXT("Clamping is true"));
		}
	}

	if (ItemData.ItemClass != nullptr)
	{
		FActorSpawnParameters Params;

		SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemData.ItemClass, LocalSpawnLocation, FRotator(0.0f, 0.0f, 0.0f), Params);

		ItemData.UseDisplayMesh = true;

		Cast<ABase_Item>(SpawnedActor)->SetInventoryData(ItemData);

		UE_LOG(LogTemp, Warning, TEXT("Item Spawned: %f"), SpawnedActor->GetActorLocation().Z);
	}
}