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

	UPROPERTY(EditAnywhere)
	FVector Acceleration;

	// ���� ����(kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// ������ ���� ���¿��� �ڵ��� ȸ�� �ݰ��� �ּ� ������(m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	
	// ������ 
	UPROPERTY(EditAnywhere)
	float Attenuation = 0.1f;

	float SteeringThrow;

	void UpdateLocationFromVelocity(const FVector& DeltaTranslation);
	//void MoveForward(float Value);
	void MoveRight(float Value);
};