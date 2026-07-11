// Copyright by Hritish Duvvur

#include "Game/Characters/ShooterCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/Combat/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement() -> MovementState.bCanCrouch = true; //Enable Crouching
	
	//Set Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(GetRootComponent());
	SpringArm -> TargetArmLength = 0.f; //First Person, so no Length is set
	SpringArm -> bEnableCameraLag = true;
	SpringArm -> CameraLagSpeed = 15.f;
	SpringArm -> bUsePawnControlRotation = true; //Rotate the arm based on controller
	
	//Set Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera -> SetupAttachment(SpringArm);
	FirstPersonCamera -> bUsePawnControlRotation = false; //Camera does not rotate relative to arm
	
	//Set Mesh
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P -> SetupAttachment(FirstPersonCamera);
	Mesh1P -> bOnlyOwnerSee = true; //Player sees the arms mesh
	Mesh1P -> bOwnerNoSee = false; //Other Players cannot see the arms mesh
	Mesh1P -> bCastDynamicShadow = false; //No Shadow
	Mesh1P -> bReceivesDecals = false;
	Mesh1P -> VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered; //Arms animation happens only for the person who sees it, so basically only for the Player
	Mesh1P -> PrimaryComponentTick.TickGroup = TG_PrePhysics; //Mesh follows Camera more smoothly, before Tick before physics simulation
	
	//Regular Mesh, that everyone sees
	GetMesh() -> bOnlyOwnerSee = false;
	GetMesh() -> bOwnerNoSee = true;
	GetMesh() -> bReceivesDecals = false;
	
	//Combat Component
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat -> SetIsReplicated(true);

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (IsValid(Combat))
	{
		Combat -> SpawnInventory();
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	//Binding Combat Input Actions
	ShooterInputComponent -> BindAction(CycleWeaponAction, ETriggerEvent::Started, this, &AShooterCharacter::Input_CycleWeapon);
	ShooterInputComponent -> BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &AShooterCharacter::Input_ReloadWeapon);
	ShooterInputComponent -> BindAction(FireWeaponAction, ETriggerEvent::Started, this, &AShooterCharacter::Input_FireWeapon_Pressed);
	ShooterInputComponent -> BindAction(FireWeaponAction, ETriggerEvent::Completed, this, &AShooterCharacter::Input_FireWeapon_Released);
	ShooterInputComponent -> BindAction(AimWeaponAction, ETriggerEvent::Started, this, &AShooterCharacter::Input_AimWeaponPressed);
	ShooterInputComponent -> BindAction(AimWeaponAction, ETriggerEvent::Completed, this, &AShooterCharacter::Input_AimWeaponReleased);
}

void AShooterCharacter::Input_CycleWeapon()
{
	Combat -> Initiate_CycleWeapon();
}

void AShooterCharacter::Input_ReloadWeapon()
{
	Combat -> Initiate_ReloadWeapon();
}

void AShooterCharacter::Input_FireWeapon_Pressed()
{
	Combat -> Initiate_FireWeapon_Pressed();
}

void AShooterCharacter::Input_FireWeapon_Released()
{
	Combat -> Initiate_FireWeapon_Released();
}

void AShooterCharacter::Input_AimWeaponPressed()
{
	Combat -> Initiate_AimWeaponPressed();
}

void AShooterCharacter::Input_AimWeaponReleased()
{
	Combat -> Initiate_AimWeaponReleased();
}

FName AShooterCharacter::GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const
{
	checkf(Combat -> WeaponData, TEXT("WeaponData is not set in CombatComponent"));
	return Combat -> WeaponData -> GripPoints.FindChecked(WeaponType);
}

USkeletalMeshComponent* AShooterCharacter::GetMesh1P_Implementation() const
{
	return Mesh1P;
}

USkeletalMeshComponent* AShooterCharacter::GetMesh3P_Implementation() const
{
	return GetMesh();
}







