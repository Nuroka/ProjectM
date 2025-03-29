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
	FVector Force = MaxDrivingForce * Throttle * GetActorForwardVector();

	Force += GetResistance();
	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;

	FVector DeltaTranslation = Velocity * DeltaTime * 100;
		

	ApplyRotation(DeltaTime);

	UpdateLocationFromVelocity(DeltaTranslation);


}

// 회전 적용
void AKart::ApplyRotation(float DeltaTime)
{
	
	float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

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
	PlayerInputComponent->BindAxis("MoveForward", this, &AKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::MoveRight);

}

FVector AKart::GetResistance()
{
	return - Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void AKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}

