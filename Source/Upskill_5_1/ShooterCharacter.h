

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	void Shoot();

	UPROPERTY(EditAnywhere)
	float Health;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void Quit();

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	
};
