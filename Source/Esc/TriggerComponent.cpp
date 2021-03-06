// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Mover.h"
#include "GameFramework/Actor.h"

UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		if (Actor->ActorHasTag("Freeze"))
		{
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (Component)
			{
				Component->SetSimulatePhysics(false);
			}
		}		
		//Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);		
	}
	else
	{
		Mover->SetShouldMove(false);
	}

}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	TotalTriggerMass = 0;

	if (Actors.Num() > 0)
	{
		for (AActor* Actor : Actors)
		{
			bool HasCorrectTag = Actor->ActorHasTag(CorrectActorTag);
			bool bIsGrabbed = Actor->ActorHasTag("Grabbed");
			if (Actor->FindComponentByClass<USceneComponent>()->IsSimulatingPhysics())
			{
				TotalTriggerMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			}
			if (HasCorrectTag && !bIsGrabbed && TotalTriggerMass >= MassRequired)
			{
				return Actor;

			}
		}
	}
	return nullptr;
}

