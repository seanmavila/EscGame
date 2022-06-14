// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector TargetLocation = OriginalLocation;
	if (ShouldMove || bIsOpen)
	{
		TargetLocation = OriginalLocation + MoveOffset;
		UE_LOG(LogTemp, Warning, TEXT("Moving"));
	}	
	float Speed = MoveOffset.Length() / MoveTime;
	FVector NewLocation = FMath::VInterpConstantTo(GetOwner()->GetActorLocation(), TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
	
}

void UMover::SetShouldMove(bool bShouldMove)
{
	ShouldMove = bShouldMove;
}

void UMover::SetShouldOpen()
{
	bIsOpen = !bIsOpen;	
}

void UMover::Unlock()
{
	bIsLocked = false;
}

bool UMover::IsLocked() const
{
	return bIsLocked;
}

FName UMover::GetUnlockTag() const
{
	return UnlockTag;
}


