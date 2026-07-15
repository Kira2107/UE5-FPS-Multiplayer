// Copyright by Hritish Duvvur

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class UAnimMontage;
class UBlendSpace;
class UAnimSequence;

//Structure for Weapon and its Anims
USTRUCT(BlueprintType)
struct FPlayerAnims
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UAnimSequence> IdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UAnimSequence> AimIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UAnimSequence> CrouchIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UAnimSequence> SprintAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UBlendSpace> AimOffset_Hip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UBlendSpace> AimOffset_Aim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UBlendSpace> Strafe_Standing;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Anims")
	TObjectPtr<UBlendSpace> Strafe_Crouching;
};

USTRUCT(BlueprintType)
struct FMontageData
{
	GENERATED_BODY()
	
	//Weapon Equip Montage
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> EquipMontage = nullptr;
	
	//Weapon Reload Montage
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> ReloadMontage = nullptr;
	
	//Weapon Fire Montage
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> FireMontage = nullptr;
};
/**
 * Data asset for defining weapon properties and behavior.
 */
UCLASS()
class FPS_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Weapons")
	TMap<FGameplayTag, FName> GripPoints; //Map for Weapon and which Socket in the Mesh it should be held
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Weapons")
	TMap<FGameplayTag, FMontageData> WeaponMontages; //Map for Weapon and its Montages
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|FirstPerson")
	TMap<FGameplayTag, FPlayerAnims> FirstPersonAnims; //Map for Weapon and its Anims
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|ThirdPerson")
	TMap<FGameplayTag, FPlayerAnims> ThirdPersonAnims; //Map for Weapon and its Anims
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|FirstPerson")
	TMap<FGameplayTag, FMontageData> FirstPersonMontage; //Map for Weapon and its First Person Montages
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|ThirdPerson")
	TMap<FGameplayTag, FMontageData> ThirdPersonMontage; //Map for Weapon and its Third Person Montages
};
