// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

UCLASS()
class KARTRACE_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyRotation(float DeltaTime);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector GetResistance();


	UPROPERTY(EditAnywhere)
	FVector Velocity;

	// 차의 질량(kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;


	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	
	
	float Throttle;
	float SteeringThrow;

	void UpdateLocationFromVelocity(const FVector& DeltaTranslation);
	void MoveForward(float Value);
	void MoveRight(float Value);
};