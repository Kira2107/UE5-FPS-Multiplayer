// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
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
	void Initiate_AimPressed();
	void Initiate_AimReleased();
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
};
