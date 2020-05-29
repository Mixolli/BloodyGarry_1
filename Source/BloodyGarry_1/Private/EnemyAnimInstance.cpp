// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (EnemyPawn == nullptr)
	{
		EnemyPawn = TryGetPawnOwner();
	}
}

void UEnemyAnimInstance::UpdateAnimationPropertiess()
{
	if (EnemyPawn == nullptr)
	{
		EnemyPawn = TryGetPawnOwner();
	}

	if (EnemyPawn)
	{
		FVector Speed = EnemyPawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		EnemyMovementSpeed = LateralSpeed.Size();
	}
}

