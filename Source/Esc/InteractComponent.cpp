// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "Mover.h"
#include "ObjectRotator.h"
#include "EscCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HasHit = LookForInteraction(HitResult);
}

bool UInteractComponent::LookForInteraction(FHitResult& OutHitResult) const
{
	// Use line trace to look for interactable object
	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = StartPoint + GetForwardVector() * MaxInteractDistance;

	return GetWorld()->SweepSingleByChannel(OutHitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(InteractRadius)); 
}

void UInteractComponent::Interaction()
{
	AActor* HitActor = HitResult.GetActor();
	AEscCharacter* Owner = Cast<AEscCharacter>(GetOwner());

	if (HasHit && HitActor->ActorHasTag("CanBeOpened"))
	{
		Mover = HitActor->FindComponentByClass<UMover>();
		if (Mover && !Mover->IsLocked())
		{
			Mover->SetShouldOpen();
		}
		else
		{
			if (Owner->IsItemInInventory(Mover->GetUnlockTag()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Unlocked..."));
				Mover->Unlock();
				Owner->RemoveItemFromInventory(Mover->GetUnlockTag());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Locked..."));
			}
		}
	}

	if (HasHit && HitActor->ActorHasTag("CanBeRotated"))
	{
		ObjectRotator = HitActor->FindComponentByClass<UObjectRotator>();
		if (ObjectRotator && !ObjectRotator->IsLocked())
		{
			ObjectRotator->SetShouldOpen();
		}
		else
		{
			if (Owner->IsItemInInventory(ObjectRotator->GetUnlockTag()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Unlocked..."));
				ObjectRotator->Unlock();
				Owner->RemoveItemFromInventory(ObjectRotator->GetUnlockTag());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Locked..."));
			}
		}
	}

	if (HasHit && HitActor->ActorHasTag("Collectible"))
	{
		//Add Item to inventory
		if (Owner)
		{
			Owner->AddItemToInventory(HitActor);
			HitActor->Destroy();
		}
	}
}

