// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UCombatComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FPS_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	
	//Combat Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> Combat;
	
	//1st Person View (Just the arms)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;
};
