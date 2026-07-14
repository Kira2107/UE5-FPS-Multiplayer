// Copyright by Hritish Duvvur

#include "Game/Characters/ShooterCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/Combat/CombatComponent.h"
#include "Game/ShooterTypes/ShooterTypes.h"
#include "Game/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	//Camera Variables
	DefaultCameraFOV = 90.f;
	
	TurningStatus = ETurningInPlace::NotTurning;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FirstPersonCamera -> SetFieldOfView(DefaultCameraFOV);
	
	//initial aim rot is calculated in begin play
	StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw, 0.f);
}

void AShooterCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (IsValid(Combat))
	{
		Combat -> DestroyInventory();
	}
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (IsValid(Combat))
	{
		Combat -> SpawnInventory();
	}
}

FRotator AShooterCharacter::GetFixedAimRotation() const
{
	FRotator AimRotation = GetBaseAimRotation();
	
	//Map Pitch from Space its in to Space we can Use
	//i.e If Pitch is from [270,360) we need to convert it to [-90,0]
	if (AimRotation.Pitch >90.f && !IsLocallyControlled())
	{
		const FVector2D InRange(270.f, 360.f);
		const FVector2D OutRange(-90.f, 0.f);
		AimRotation.Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimRotation.Pitch);
	}
	return AimRotation;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CalculateTurnInPlaceParameters(DeltaTime);
	CalculateFABRIKSocketTransform();
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
	OnAim(true);
}

void AShooterCharacter::Input_AimWeaponReleased()
{
	Combat -> Initiate_AimWeaponReleased();
	OnAim(false);
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

bool AShooterCharacter::HasCurrentWeapon() const
{
	return IsValid(Combat) && Combat -> CurrentWeapon != nullptr;
}

void AShooterCharacter::CalculateFABRIKSocketTransform()
{
	if (IsValid(Combat) && IsValid(Combat -> CurrentWeapon) && IsValid(Combat -> CurrentWeapon -> GetMesh3P()))
	{
		//Get the FABRIK Socket Transform
		FABRIK_SocketTransform = Combat -> CurrentWeapon -> GetMesh3P() -> GetSocketTransform(FName("FABRIK_Socket", RTS_World));
		
		//Transform to Bone Space as FABRIK Needs Bone Transform
		FVector OutLocation;
		FRotator OutRotation;
		GetMesh() -> TransformToBoneSpace("hand_r", FABRIK_SocketTransform.GetLocation(), FABRIK_SocketTransform.GetRotation().Rotator(), OutLocation, OutRotation);
		FABRIK_SocketTransform.SetLocation(OutLocation);
		FABRIK_SocketTransform.SetRotation(OutRotation.Quaternion());
	}
}

void AShooterCharacter::CalculateTurnInPlaceParameters(float DeltaTime)
{
	// Get Velocity to see if we are running, if 0 we are still
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size2D();
	
	//See if we are falling
	bool bIsInAir = GetCharacterMovement() -> IsFalling();
	
	//If standing still and not jumping
	if (Speed == 0.f && !bIsInAir)
	{
		//get current aim rotation
		FRotator CurrentAimRotation(0.f, GetBaseAimRotation().Yaw, 0.f);
		
		//delta aim rotation - diff in current aim from initial aim
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		
		//Store yaw of the delta aim rotation (AO_Yaw)
		AO_Yaw = DeltaAimRotation.Yaw;
		
		if(TurningStatus == ETurningInPlace::NotTurning)
		{
			//Set InterpAO_Yaw to AO_Yaw
			InterpAO_Yaw = AO_Yaw;
		}
		
		TurnInPlace(DeltaTime); //Interp the InterpAO_Yaw value to 0
	}
		
	if (Speed > 0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		
		FRotator AimRotation = GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
		TurningStatus = ETurningInPlace::NotTurning;
	}
	
	AO_Yaw *= -1.f;
}

void AShooterCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		//TurningStatus = TurningRight
		TurningStatus = ETurningInPlace::TurningRight;
	}
	else if (AO_Yaw < -90.f)
	{
		//TurningStatus = TurningLeft
		TurningStatus = ETurningInPlace::TurningLeft;
	}
	
	if (TurningStatus != ETurningInPlace::NotTurning) //We are Turning, so time to interp
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		
		if (FMath::Abs(AO_Yaw) < 5.f)
		{
			TurningStatus = ETurningInPlace::NotTurning;
			
			//Reset initial aim rot to actual aim rot
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}








