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
	
	virtual void OnRep_Instigator() override;
	
	//Getter for Meshes
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	USkeletalMeshComponent* GetMesh3P() const { return Mesh3P; }
	
	//
	void AttachToOwningPawn() const;
	
	//Weapon Type Tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPS|WeaponType")
	FGameplayTag WeaponType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	/* Functions */
	
	//Change Weapon Visibility based on Local Player or Not
	void SetMeshVisibilities(APawn* OwningPawn) const;
	
	
	/* Variables */
	
	//Weapon Mesh: First Person View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	//Weapon Mesh: Third Person View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
};
