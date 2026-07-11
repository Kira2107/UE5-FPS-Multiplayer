// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/Data/WeaponData.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* Public Functions */
	
	// Sets default values for this component's properties
	UCombatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Combat Behavior Functions
	
	//Cycle to the Next Weapon in Inventory
	void Initiate_CycleWeapon();
	
	//Weapon Firing Behaviors
	void Initiate_FireWeapon_Pressed();
	void Initiate_FireWeapon_Released();
	
	//Weapon Reload
	void Initiate_ReloadWeapon();
	
	//Weapon Aim
	void Initiate_AimWeaponPressed();
	void Initiate_AimWeaponReleased();
	
	//
	void SpawnInventory();
	
	void DestroyInventory();
	
	
	
	/* Public Variables */
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	TObjectPtr<UWeaponData> WeaponData;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	//
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const;
	
};
