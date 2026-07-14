#pragma once

#include "ShooterTypes.generated.h"

UENUM(BlueprintType)
enum class ETurningInPlace :uint8
{
	NotTurning UMETA(DisplayName = "NotTurning"),
	TurningLeft UMETA(DisplayName = "TurningLeft"),
	TurningRight UMETA(DisplayName = "TurningRight"),
};