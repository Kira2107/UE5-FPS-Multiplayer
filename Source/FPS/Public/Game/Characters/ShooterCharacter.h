// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "Game/Interfaces/PlayerInterface.h"
#include "Game/ShooterTypes/ShooterTypes.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UInputAction;
class UCombatComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class FPS_API AShooterCharacter : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	/* Public Functions */
	
	// Sets default values for this character's properties
	AShooterCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void BeginDestroy() override;
	
	//Called when character is possessed
	virtual void PossessedBy(AController* NewController) override;
	
	//Fix the Characters Aim Rotation when Replicated in MultiPlayer
	UFUNCTION(BlueprintCallable)
	FRotator GetFixedAimRotation() const;
	
	/* Player Interface Functions */
	virtual FName GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const override;
	virtual USkeletalMeshComponent* GetMesh1P_Implementation() const override;
	virtual USkeletalMeshComponent* GetMesh3P_Implementation() const override;
	/* Player Interface Functions End*/
	
	
	/* Public Variables */
	
	//Combat Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPS|Combat")
	TObjectPtr<UCombatComponent> Combat;
	
	//Return FABRIK Socket on the Weapon in Bone Space
	UPROPERTY(BlueprintReadOnly, Category="FPS|FABRIK")
	FTransform FABRIK_SocketTransform;
	
	//To fix ABP and to now have FABRIK break the model if we dont have a weapon
	UFUNCTION(BlueprintCallable)
	bool HasCurrentWeapon() const;
	
	
protected:
	/* Protected Functions */
	
	
	/* Protected Variables */
	
	//Camera Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPS|Camera")
	float DefaultCameraFOV;
	
	//Function to call when ADS is being used
	UFUNCTION(BlueprintImplementableEvent)
	void OnAim(bool bIsAiming);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPS|Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	//
	UPROPERTY(BlueprintReadOnly, Category="FPS|TurnInPlace")
	float AO_Yaw;
	
	//
	UPROPERTY(BlueprintReadOnly, Category="FPS|Strafing")
	float MovementOffsetYaw;
	
	//Tunrnig in Place Enum
	UPROPERTY(BlueprintReadOnly, Category="FPS|TurnInPlace")
	ETurningInPlace TurningStatus;
	
private:
	//Functions
	
	//Input Callback Functions
	void Input_CycleWeapon();
	void Input_ReloadWeapon();
	void Input_FireWeapon_Pressed();
	void Input_FireWeapon_Released();
	void Input_AimWeaponPressed();
	void Input_AimWeaponReleased();
	
	//Function to Calculate Socket Transform
	void CalculateFABRIKSocketTransform();
	
	//
	void CalculateTurnInPlaceParameters(float DeltaTime);
	
	//
	void TurnInPlace(float DeltaTime);
	
	
	//Variables
	
	//Combat Weapon Input Actions
	UPROPERTY(EditAnywhere, Category="FPS|Input")
	TObjectPtr<UInputAction> CycleWeaponAction;
	
	UPROPERTY(EditAnywhere, Category="FPS|Input")
	TObjectPtr<UInputAction> FireWeaponAction;
	
	UPROPERTY(EditAnywhere, Category="FPS|Input")
	TObjectPtr<UInputAction> ReloadWeaponAction;
	
	UPROPERTY(EditAnywhere, Category="FPS|Input")
	TObjectPtr<UInputAction> AimWeaponAction;
	
	//1st Person View (Just the arms)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	//Initial Aim Rotation
	FRotator StartingAimRotation;
	
	float InterpAO_Yaw;

};
