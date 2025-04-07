// Fill out your copyright notice in the Description page of Project Settings.


#include "Kart.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "GameFramework/GameStateBase.h"

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
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}


FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		FKartMove Move = CreateMove(DeltaTime);

		// 권한이 없으면 = 클라이언트에 큐
		if (!HasAuthority())
		{
			// 큐(이동 대기열)에 움직임 저장 
			UnacknowledgedMoves.Add(Move);
		}


		Server_SendMove(Move);

		SimulateMove(Move);
	}

}

// 회전 적용
void AKart::ApplyRotation(float DeltaTime, float SteeringThrow)
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
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::Move_Right);

}

FVector AKart::GetResistance()
{
	return - Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void AKart::Move_Right(float Value)
{
	HandleSteeringThrow = Value;
}

void AKart::Server_SendMove_Implementation(FKartMove Move)
{
	SimulateMove(Move);

	ServerState.LastMove = Move;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
	// 마지막 동작, 움직인 시뮬 상태를 보냄
	//ServerState.LastMove =
}

// -1 ~ 1사이의 값이 아닌지 확인
bool AKart::Server_SendMove_Validate(FKartMove Move)
{
	return true;
}


// 
void AKart::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKart, ServerState);
}

void AKart::OnRep_ServerState()
{
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgeMoves(ServerState.LastMove);


	for (const FKartMove& Move : UnacknowledgedMoves)
	{
		SimulateMove(Move);
	}
}

// Move 시뮬레이팅 
void AKart::SimulateMove(const FKartMove& Move)
{
	// 시간이 지나면 지날수록 빨라지도록
	//float TimeElapsed = GetWorld()->GetTimeSeconds();

	// 힘(F) 계산 = 감쇠 * 점점 빨라지게 시간
	FVector Force = MaxDrivingForce * GetActorForwardVector() * Attenuation * Move.Time;

	// 공기저항
	Force += GetResistance();

	// a = F/M
	Acceleration = Force / Mass;
	
	Velocity += Acceleration * Move.DeltaTime;

	FVector DeltaTranslation = Velocity * Move.DeltaTime * 100;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(DeltaTranslation);
}

FKartMove AKart::CreateMove(float DeltaTime)
{
	FKartMove Move;

	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = HandleSteeringThrow;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

void AKart::ClearAcknowledgeMoves(FKartMove LastMove)
{
	TArray<FKartMove> NewMoves;

	for (const FKartMove& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;

}
