// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KARTRACE_API UKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FKartMove& Move);
	FKartMove CreateMove(float DeltaTime);

	FVector GetVelocity() { return Velocity; };
	void SetVelocity(FVector Val) { Velocity = Val; };

	void SetHandleSteeringThrow(float Val) { HandleSteeringThrow = Val; };



private:
		

	FVector GetResistance();

	void ApplyRotation(float DeltaTime, float SteeringThrow);



	UFUNCTION()

	void UpdateLocationFromVelocity(const FVector& DeltaTranslation);

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
	float Attenuation = 1.7f;
	
	
	FVector Velocity;

	// �ڵ� ����
	float HandleSteeringThrow;

};
