// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Base_Item.generated.h"

class UCapsuleComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		uint8 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		uint8 SizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		UMaterialInterface* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		UMaterialInterface* IconRotated = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		bool Rotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		bool Stackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		int32 CurrentStack = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory Data")
		bool UseDisplayMesh = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Data")
		TSubclassOf<ABase_Item> ItemClass = nullptr;
};

UCLASS(BlueprintType)
class CATACLYSM_API ABase_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Item();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		FInventoryData GetInventoryData();

	UFUNCTION(BlueprintCallable)
		void SetInventoryData(FInventoryData ItemDataIN);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"), Replicated)
		FInventoryData InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DisplayMesh;

private:

};