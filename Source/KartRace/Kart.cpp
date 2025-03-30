// Fill out your copyright notice in the Description page of Project Settings.


#include "Kart.h"
#include "math.h"
#include "Components/InputComponent.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 시간이 지나면 지날수록 빨라지도록
	float TimeElapsed = GetWorld()->GetTimeSeconds();

	// 힘(F) 계산
	FVector Force = MaxDrivingForce * TimeElapsed * GetActorForwardVector() * Attenuation;

	// 공기저항
	Force += GetResistance(); 
	
	// a = F/M
	Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaTime;

	FVector DeltaTranslation = Velocity * DeltaTime * 100;

	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTranslation);
}

// 회전 적용
void AKart::ApplyRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity)* DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

// Hit 이벤트 설정
void AKart::UpdateLocationFromVelocity(const FVector& DeltaTranslation)
{
	// 부딪쳤을때 설정
	FHitResult Hit;
	AddActorWorldOffset(DeltaTranslation, true, &Hit);

	// 부딪쳤을때 속도 0으로 재설정
	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

	// 죽었을때 설정
}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("MoveForward", this, &AKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::MoveRight);

}

FVector AKart::GetResistance()
{
	return - Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

//void AKart::MoveForward(float Value)
//{
//	Throttle = Value;
//}

void AKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}

