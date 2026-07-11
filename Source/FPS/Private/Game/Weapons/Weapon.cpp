// Copyright by Hritish Duvvur


#include "Game/Weapons/Weapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Game/Interfaces/PlayerInterface.h"
#include "GameFramework/Pawn.h"


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

void AWeapon::OnRep_Instigator()
{
	Super::OnRep_Instigator();
	
	AttachToOwningPawn();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::SetMeshVisibilities(APawn* OwningPawn) const
{
	if (OwningPawn -> IsLocallyControlled())
	{
		Mesh1P -> SetHiddenInGame(false);
		Mesh3P -> SetHiddenInGame(true);
	}
	else
	{
		Mesh1P -> SetHiddenInGame(true);
		Mesh3P -> SetHiddenInGame(false);
	}
}

void AWeapon::AttachToOwningPawn() const
{
	APawn* OwningPawn = GetInstigator();
	if (!IsValid(OwningPawn) || !OwningPawn->Implements<UPlayerInterface>()) return;
	
	SetMeshVisibilities(OwningPawn);
	
	//Get Attach Points
	const FName AttachPoint = IPlayerInterface::Execute_GetWeaponAttachPoint(OwningPawn, WeaponType);
	USkeletalMeshComponent* PawnMesh1P = IPlayerInterface::Execute_GetMesh1P(OwningPawn);
	USkeletalMeshComponent* PawnMesh3P = IPlayerInterface::Execute_GetMesh3P(OwningPawn);
	
	//Attach to Socket
	Mesh1P -> AttachToComponent(PawnMesh1P, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
	Mesh3P -> AttachToComponent(PawnMesh3P, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
}





