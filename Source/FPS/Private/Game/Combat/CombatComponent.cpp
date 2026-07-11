// Copyright by Hritish Duvvur


#include "Game/Combat/CombatComponent.h"

#include "Engine/Engine.h"
#include "Game/Weapons/Weapon.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"


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

void UCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCombatComponent, Inventory);
	DOREPLIFETIME(UCombatComponent, CurrentWeapon);
}

void UCombatComponent::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon -> AttachToOwningPawn();
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

AWeapon* UCombatComponent::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor)) return nullptr;
	if (OwningActor -> GetLocalRole() < ROLE_Authority) return nullptr; //Has Authority to Spawn
	
	//Set Info to Spawn Weapon
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//Return Weapon to Spawn and its Info
	return GetWorld() -> SpawnActor<AWeapon>(WeaponClass, SpawnInfo);
}

void UCombatComponent::SpawnInventory()
{
	if (GetOwner() -> GetLocalRole() < ROLE_Authority) return; //Has Authority to Spawn
	
	for (TSubclassOf<AWeapon>& WeaponClass : DefaultWeaponClasses)
	{
		AWeapon* NewWeapon = SpawnWeapon(WeaponClass);
		if (IsValid(NewWeapon))
		{
			Inventory.AddUnique(NewWeapon);
		}
	}
	
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void UCombatComponent::DestroyInventory()
{
	for (AWeapon* Weapon : Inventory)
	{
		if (IsValid(Weapon))
		{
			Weapon -> Destroy();
		}
	}
	Inventory.Empty();
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	CurrentWeapon = WeaponToEquip;
	CurrentWeapon -> AttachToOwningPawn();
}

