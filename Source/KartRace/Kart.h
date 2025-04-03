// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

// 서버로 보내기 위한 구조체 (시뮬레이션)
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

	// 차의 질량(kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// 완전히 돌린 상태에서 자동차 회전 반경의 최소 반지름(m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	
	// 감쇠계수 
	UPROPERTY(EditAnywhere)
	float Attenuation = 0.1f;

	// Server RPC(원격프로시저 호출)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FKartMove Move);
	
	// 핸들 꺾기
	float HandleSteeringThrow;

	void Move_Right(float Value);

	void UpdateLocationFromVelocity(const FVector& DeltaTranslation);
};