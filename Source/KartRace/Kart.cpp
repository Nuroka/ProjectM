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

	// �ð��� ������ �������� ����������
	float TimeElapsed = GetWorld()->GetTimeSeconds();

	// ��(F) ���
	FVector Force = MaxDrivingForce * TimeElapsed * GetActorForwardVector() * Attenuation;

	// ��������
	Force += GetResistance(); 
	
	// a = F/M
	Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaTime;

	FVector DeltaTranslation = Velocity * DeltaTime * 100;

	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTranslation);
}

// ȸ�� ����
void AKart::ApplyRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity)* DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

// Hit �̺�Ʈ ����
void AKart::UpdateLocationFromVelocity(const FVector& DeltaTranslation)
{
	// �ε������� ����
	FHitResult Hit;
	AddActorWorldOffset(DeltaTranslation, true, &Hit);

	// �ε������� �ӵ� 0���� �缳��
	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

	// �׾����� ����
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

