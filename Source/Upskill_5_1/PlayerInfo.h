#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType) struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FText PlayerName = FText::FromString("Unknown Name");
	UPROPERTY(BlueprintReadOnly) bool bIsReady = false;
	UPROPERTY(BlueprintReadOnly) APawn* Character = nullptr;
};
