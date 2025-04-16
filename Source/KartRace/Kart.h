// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartMovementComponent.h"
#include "Kart.generated.h"

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FKartState ServerState;

	// Server RPC(원격프로시저 호출)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FKartMove Move);
	
	void Move_Right(float Value);

	void ClearAcknowledgeMoves(FKartMove LastMove);

	UFUNCTION()
	void OnRep_ServerState();

	TArray<FKartMove> UnacknowledgedMoves;

	UPROPERTY(EditAnywhere)
	UKartMovementComponent* MovementComponent;

};