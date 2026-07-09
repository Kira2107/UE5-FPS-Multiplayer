// Copyright by Hritish Duvvur


#include "Game/Combat/CombatComponent.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::Initiate_CycleWeapon()
{
}

void UCombatComponent::Initiate_FireWeapon_Pressed()
{
}

void UCombatComponent::Initiate_FireWeapon_Released()
{
}

void UCombatComponent::Initiate_ReloadWeapon()
{
}

void UCombatComponent::Initiate_AimPressed()
{
}

void UCombatComponent::Initiate_AimReleased()
{
}

