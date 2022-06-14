// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectRotator.h"

// Sets default values for this component's properties
UObjectRotator::UObjectRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectRotator::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetOwner()->GetActorRotation();
	
}


// Called every frame
void UObjectRotator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator TargetRotation = OriginalRotation;

	if (bIsOpen)
	{
		TargetRotation = FRotator(OriginalRotation.Pitch + PitchOffset, OriginalRotation.Yaw + YawOffset, OriginalRotation.Roll + RollOffset);
	}
	FRotator NewRotation = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, RotateTime);
	FRotator ObjectRotation = GetOwner()->GetActorRotation();
	ObjectRotation = NewRotation;
	GetOwner()->SetActorRotation(ObjectRotation);
}

void UObjectRotator::SetShouldOpen()
{
	bIsOpen = !bIsOpen;
}

void UObjectRotator::Unlock()
{
	bIsLocked = false;
}

bool UObjectRotator::IsLocked() const
{
	return bIsLocked;
}

FName UObjectRotator::GetUnlockTag() const
{
	return UnlockTag;
}

