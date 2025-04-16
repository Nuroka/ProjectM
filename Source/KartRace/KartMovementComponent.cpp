// Fill out your copyright notice in the Description page of Project Settings.

#include "KartMovementComponent.h"
#include "GameFramework/GameStateBase.h"

// Sets default values for this component's properties
UKartMovementComponent::UKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// Move �ùķ����� 
void UKartMovementComponent::SimulateMove(const FKartMove& Move)
{
	float TimeFactor = 1.0f / (1.0f + FMath::Exp(-Move.Time));

	// ��(F) ��� = ���� * ���� �������� �ð�
	FVector Force = MaxDrivingForce * GetOwner()->GetActorForwardVector() * Attenuation * TimeFactor;
	
	// ��������
	Force += GetResistance();

	// a = F/M
	Acceleration = Force / Mass;

	Velocity += Acceleration * Move.DeltaTime;

	FVector DeltaTranslation = Velocity * Move.DeltaTime * 100;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(DeltaTranslation);
}

FKartMove UKartMovementComponent::CreateMove(float DeltaTime)
{
	FKartMove Move;

	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = HandleSteeringThrow;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

// ȸ�� ����
void UKartMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

// Hit �̺�Ʈ ����
void UKartMovementComponent::UpdateLocationFromVelocity(const FVector& DeltaTranslation)
{
	// �ε������� ����
	FHitResult Hit;
	GetOwner()->AddActorWorldOffset(DeltaTranslation, true, &Hit);

	// �ε������� �ӵ� 0���� �缳��
	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

	// �׾����� ����
}


FVector UKartMovementComponent::GetResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}