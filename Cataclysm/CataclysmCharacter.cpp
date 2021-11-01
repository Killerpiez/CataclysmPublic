// Copyright Epic Games, Inc. All Rights Reserved.

#include "CataclysmCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InventorySystem/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

#include "DrawDebugHelpers.h"

ACataclysmCharacter::ACataclysmCharacter()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Create 3D Cursor
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	// Set up default inventory component (later this might be moved to an item instead of the character)
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	AddOwnedComponent(InventoryComponent);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
}

void ACataclysmCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACataclysmCharacter, LookingDirection, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(ACataclysmCharacter, VelocityDirection, COND_SimulatedOnly);
}

void ACataclysmCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerReference = Cast<APlayerController>(GetController());
}

void ACataclysmCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &ACataclysmCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACataclysmCharacter::MoveRight);

}

void ACataclysmCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (PlayerControllerReference != nullptr)
	{
		FaceDirection(UpdateCursor(PlayerControllerReference));
	}

	// Debug String. <Remove>
	DrawDebugString(GetWorld(), FVector(75, -150, 0), MyGetRoleName(GetLocalRole()), this, FColor::Black, DeltaSeconds);
}

// Returns World Location of the cursor
FVector ACataclysmCharacter::UpdateCursor(APlayerController* PlayerController)
{
	if (CursorToWorld == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CursorToWorld is NULL on Character"));
		return FVector(0,0,0);
	}

	FHitResult TraceHitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	FVector CursorFV = TraceHitResult.ImpactNormal;
	FRotator CursorR = CursorFV.Rotation();
	CursorToWorld->SetWorldLocation(TraceHitResult.Location);
	CursorToWorld->SetWorldRotation(CursorR);

	// Shows owning player their cursor
	if (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_Authority)
	{
		CursorToWorld->SetVisibility(true);
	}

	return CursorToWorld->GetComponentTransform().GetLocation();
}

void ACataclysmCharacter::MoveForward(float val)
{
	//set forward vector
	const FVector Direction(1, 0, 0);

	//Start Move Forward
	AddMovementInput(Direction, val);
}

void ACataclysmCharacter::MoveRight(float val)
{
	//set right vector
	const FVector Direction(0, 1, 0);

	//Start Move Right
	AddMovementInput(Direction, val);
}

void ACataclysmCharacter::FaceDirection(FVector CursorWorldLocation)
{
	// This only updates Autonmous Proxy
	// Set Player Rotation 
	LookingDirection = FVector(FVector(CursorWorldLocation.X - GetActorLocation().X, CursorWorldLocation.Y - GetActorLocation().Y, 0)).Rotation();

	// We only want the player to turn toward the target if he's moving
	if (GetVelocity().SizeSquared() != 0.0f)
	{
		SetActorRotation(LookingDirection);
	}

	// Calculate traveling velocity direction for animation graph
	FVector Velocity = GetVelocity();
	Velocity.Normalize();
	VelocityDirection = FVector(FVector(Velocity.X, Velocity.Y, 0)).Rotation();

	Server_FaceDirection(CursorWorldLocation);

}

void ACataclysmCharacter::Server_FaceDirection_Implementation(FVector CursorWorldLocation)
{
	// Set Player Rotation 
	LookingDirection = FVector(FVector(CursorWorldLocation.X - GetActorLocation().X, CursorWorldLocation.Y - GetActorLocation().Y, 0)).Rotation();

	// We only want the player to turn toward the target if he's moving
	if (GetVelocity().SizeSquared() != 0.0f)
	{
		SetActorRotation(LookingDirection);
	}

	// Calculate traveling velocity direction for animation graph
	FVector Velocity = GetVelocity();
	Velocity.Normalize();
	VelocityDirection = FVector(FVector(Velocity.X, Velocity.Y, 0)).Rotation();
}

bool ACataclysmCharacter::Server_FaceDirection_Validate(FVector CursorWorldLocation)
{
	// As of now I cant think of a way that the cursor world location can cause the looking direction to cheat. May need to adjust this later
	return true;
}

// Returns the role name as a string. Used right now only for the debug string. May need it for switch cases.
FString ACataclysmCharacter::MyGetRoleName(ENetRole MyRole)
{
	switch (MyRole)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "Simulate Proxy";
	case ROLE_AutonomousProxy:
		return "Autonomous Proxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "Max";
	default:
		return "None";
	}
}

UInventoryComponent* ACataclysmCharacter::GetInventoryComponent()
{
	return InventoryComponent;
}