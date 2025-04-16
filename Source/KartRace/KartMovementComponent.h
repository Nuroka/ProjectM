// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.generated.h"

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
	float Attenuation = 1.7f;
	
	
	FVector Velocity;

	// 핸들 꺾기
	float HandleSteeringThrow;

};
