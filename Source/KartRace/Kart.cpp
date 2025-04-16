// Fill out your copyright notice in the Description page of Project Settings.


#include "Kart.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MovementComponent = CreateDefaultSubobject<UKartMovementComponent>(TEXT("MovementComponent"));
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

	if (MovementComponent == nullptr) return;

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FKartMove Move = MovementComponent->CreateMove(DeltaTime);
		MovementComponent->SimulateMove(Move);

		UnacknowledgedMoves.Add(Move);
		Server_SendMove(Move);
	}
	// 내가 서버고 폰을 컨트롤함
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		FKartMove Move = MovementComponent->CreateMove(DeltaTime);
		Server_SendMove(Move);

	}
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState.LastMove);
	}

}


void AKart::Move_Right(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetHandleSteeringThrow(Value);
}

void AKart::Server_SendMove_Implementation(FKartMove Move)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SimulateMove(Move);

	ServerState.LastMove = Move;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
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
	if (MovementComponent == nullptr) return;

	SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgeMoves(ServerState.LastMove);


	for (const FKartMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("MoveForward", this, &AKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::Move_Right);

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