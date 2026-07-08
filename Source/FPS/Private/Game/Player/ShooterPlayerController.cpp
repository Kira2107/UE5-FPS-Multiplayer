// Copyright by Hritish Duvvur


#include "Game/Player/ShooterPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


AShooterPlayerController::AShooterPlayerController()
{
	bReplicates = true;
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); //Get the Enhanced Input Subsystem for the Local Player
	if (IsValid(Subsystem))
	{
		Subsystem -> AddMappingContext(ShooterIMC, 0); //Add the Input Mapping Context to the Subsystem with a priority of 0
	}
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); //Cast to Enhanced Input Component
	ShooterInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Input_Move); //Bind Move Action
	ShooterInputComponent -> BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Input_Look); //Bind Look Action
	ShooterInputComponent -> BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterPlayerController::Input_Jump); //Bind Jump Action
	ShooterInputComponent -> BindAction(CrouchAction, ETriggerEvent::Started, this, &AShooterPlayerController::Input_Crouch); //Bind Crouch Action
}

void AShooterPlayerController::Input_Crouch()
{
	//Set a bool on Character Movement Component, ensures that everything is replicated and predicted
	if (!IsValid(GetCharacter())) return;
	
	if (UCharacterMovementComponent* CMC = GetCharacter() -> GetCharacterMovement(); IsValid(CMC))
	{
		CMC -> bWantsToCrouch = !CMC -> bWantsToCrouch; //Toggle Crouch
	}
}

void AShooterPlayerController::Input_Jump()
{
	if (!IsValid(GetCharacter())) return;
	UCharacterMovementComponent* CMC = GetCharacter() -> GetCharacterMovement(); 
	if (!IsValid(CMC)) return;

	if (CMC -> bWantsToCrouch)
	{
		CMC -> bWantsToCrouch = false; //Uncrouch if crouching
	}
	else
	{
		GetCharacter() -> Jump(); //Jump if not crouching
	}
}

void AShooterPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void AShooterPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Get the forward direction vector based on the yaw rotation
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Get the right direction vector based on the yaw rotation
	
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn -> AddMovementInput(ForwardDirection, InputAxisVector.Y); //Move Forward/Backward
		ControlledPawn -> AddMovementInput(RightDirection, InputAxisVector.X); //Move Right/Left
	}
}
