#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType) struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FText PlayerName;
	UPROPERTY(BlueprintReadOnly) bool bIsReady;
	UPROPERTY(BlueprintReadOnly) APawn* Character;
};
