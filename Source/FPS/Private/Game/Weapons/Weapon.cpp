// Copyright by Hritish Duvvur


#include "Game/Weapons/Weapon.h"

#include "Components/SkeletalMeshComponent.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	bNetUseOwnerRelevancy = true; //If you can see the Owner, you can see and replicate the Weapon
	
	//First Person Mesh Settings
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P -> VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered; //Arms animation happens only for the person who sees it, so basically only for the Player
	Mesh1P -> bReceivesDecals = false;
	Mesh1P -> CastShadow = false;
	Mesh1P -> SetHiddenInGame(true);
	SetRootComponent(Mesh1P);
	
	//Third Person Mesh Settings
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3P"));
	Mesh3P -> VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered; //Arms animation happens only for the person who sees it, so basically only for the Player
	Mesh3P -> bReceivesDecals = false;
	Mesh3P -> CastShadow = true;
	Mesh3P -> SetupAttachment(Mesh1P);
	Mesh3P -> SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}



