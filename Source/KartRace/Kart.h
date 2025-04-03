// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

// ������ ������ ���� ����ü (�ùķ��̼�)
USTRUCT()
struct FKartMove
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FKartMove LastMove;

};



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

	void ApplyRotation(float DeltaTime, float SteeringThrow);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SimulateMove(FKartMove Move);

	FVector GetResistance();

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FKartState ServerState;
	
	
	UFUNCTION()
	void OnRep_ServerState();
	


	UPROPERTY(Replicated)
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

	// Server RPC(�������ν��� ȣ��)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FKartMove Move);
	
	// �ڵ� ����
	float HandleSteeringThrow;

	void Move_Right(float Value);

	void UpdateLocationFromVelocity(const FVector& DeltaTranslation);
};