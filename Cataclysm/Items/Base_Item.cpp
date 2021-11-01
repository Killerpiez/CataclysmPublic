// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Item.h"
#include "Cataclysm/CataclysmCharacter.h"
#include "Cataclysm/InventorySystem/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABase_Item::ABase_Item()
{
	// Create Collision Mesh
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionCapsule"));
	RootComponent = CollisionComponent;

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(RootComponent);


	CollisionComponent->SetSphereRadius(55.0f, true);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABase_Item::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABase_Item::OnOverlapEnd);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void ABase_Item::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABase_Item, InventoryData, COND_SimulatedOnly);
}

// Called when the game starts or when spawned
void ABase_Item::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABase_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Item::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACataclysmCharacter* ActivatingActor = Cast<ACataclysmCharacter>(OtherActor);
	if (ActivatingActor)
	{
		if (ActivatingActor->GetInventoryComponent()->TryAddItem(GetInventoryData(), false))
		{
			if (ActivatingActor->GetInventoryComponent()->OnInventoryChanged.IsBound())
			{
				// Widget_InventoryGrid is listening
				ActivatingActor->GetInventoryComponent()->OnInventoryChanged.Broadcast();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Base_Item: Event 'OnInventoryChanged' not bound"));
			}

			// Destroy the item thats been picked up
			Destroy();
		}
		else
		{
			ActivatingActor->GetInventoryComponent()->RotateItem(InventoryData);

			if (ActivatingActor->GetInventoryComponent()->TryAddItem(GetInventoryData(), false))
			{
				if (ActivatingActor->GetInventoryComponent()->OnInventoryChanged.IsBound())
				{
					// Widget_InventoryGrid is listening
					ActivatingActor->GetInventoryComponent()->OnInventoryChanged.Broadcast();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Base_Item: Event 'OnInventoryChanged' not bound"));
				}

				// Destroy the item thats been picked up
				Destroy();
			}
		}
	}
}

void ABase_Item::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACataclysmCharacter* ActivatingActor = Cast<ACataclysmCharacter>(OtherActor);
	if (ActivatingActor)
	{
		// When I switch the pick up method to be on clicked instead of overlap only, I will use this function to handle unregistering available interactable objects.
	}
}

FInventoryData ABase_Item::GetInventoryData()
{
	return InventoryData;
}

void ABase_Item::SetInventoryData(FInventoryData ItemDataIN)
{
	InventoryData = ItemDataIN;
}
