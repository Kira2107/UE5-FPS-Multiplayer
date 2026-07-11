// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	
	//Getter for Meshes
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	USkeletalMeshComponent* GetMesh3P() const { return Mesh3P; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Weapon Type Tag
	UPROPERTY(EditAnywhere, Category="FPS|WeaponType")
	FGameplayTag WeaponType;

private:
	
	//Weapon Mesh: First Person View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	//Weapon Mesh: Third Person View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
};
