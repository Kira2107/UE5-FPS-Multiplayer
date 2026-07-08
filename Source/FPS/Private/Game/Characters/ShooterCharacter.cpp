// Copyright by Hritish Duvvur

#include "Game/Characters/ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

