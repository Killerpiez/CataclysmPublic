// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CataclysmCharacter.generated.h"

class APlayerController;
class ABase_Item;
class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class UInventoryComponent;

UCLASS(Blueprintable)
class ACataclysmCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACataclysmCharacter();

	// A decal that projects to the cursor location.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UFUNCTION()
		UInventoryComponent* GetInventoryComponent();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:

	// Move Function
	UFUNCTION()
		void MoveForward(float val);

	// Move Function
	UFUNCTION()
		void MoveRight(float val);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_FaceDirection(FVector CursorWorldLocation);

	UFUNCTION()
		void FaceDirection(FVector CursorWorldLocation);

	// Debug Function
	UFUNCTION()
		FString MyGetRoleName(ENetRole MyRole);

	UFUNCTION()
		FVector UpdateCursor(APlayerController* PlayerController);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

private:
	// Top down camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	// Camera boom positioning the camera above the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	UPROPERTY()
		APlayerController* PlayerControllerReference;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
		FRotator LookingDirection = FRotator(0.0, 0.0, 0.0);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
		FRotator VelocityDirection = FRotator(0.0, 0.0, 0.0);
};