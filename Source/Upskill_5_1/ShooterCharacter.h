

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class UPSKILL_5_1_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	
};
