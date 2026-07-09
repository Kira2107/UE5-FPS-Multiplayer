// Copyright by Hritish Duvvur


#include "Game/Combat/CombatComponent.h"

#include "Engine/Engine.h"


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
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Cycle Weapon Initiated"));
}

void UCombatComponent::Initiate_FireWeapon_Pressed()
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fire Weapon Pressed"));
}

void UCombatComponent::Initiate_FireWeapon_Released()
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fire Weapon Released"));
}

void UCombatComponent::Initiate_ReloadWeapon()
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reload Weapon Initiated"));
}

void UCombatComponent::Initiate_AimWeaponPressed()
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aim Weapon Pressed"));
}

void UCombatComponent::Initiate_AimWeaponReleased()
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aim Weapon Released"));
}

