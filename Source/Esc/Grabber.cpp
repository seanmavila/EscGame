// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDist;

		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber requires a UPhysicsHandleComponent"));
	}
	return Result;
}

bool UGrabber::GetGrabbleInReach(FHitResult& OutHitResult) const
{
	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = StartPoint + GetForwardVector() * MaxGrabDistance;

	return GetWorld()->SweepSingleByChannel(OutHitResult, StartPoint, EndPoint, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GrabRadius));
}

void UGrabber::ExtendRangeOfObject()
{
	
	HoldDist = FMath::Min(HoldDist + ExtensionSpeed, 1000);
	
}

void UGrabber::RetractRangeOfObject()
{
	HoldDist = FMath::Max(HoldDist - ExtensionSpeed, 200);
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	FHitResult HitResult;
	bool HasHit = GetGrabbleInReach(HitResult);

	if (HasHit)
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		HitComp->SetSimulatePhysics(true);
		HitComp->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		AActor* HitActor = HitResult.GetActor();
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComp, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::RotateActor(float Pitch, float Yaw, float Roll)
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsControl(false);
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		RotationRate = FRotator(Pitch, Yaw, Roll);
		GrabbedActor->AddActorWorldRotation(RotationRate * GetWorld()->DeltaTimeSeconds * RotationSpeed);
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDist;
		GrabbedActor->SetActorLocation(TargetLocation);
	}
}

void UGrabber::PhysicsControl(bool bIsPhysicsActive)
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(bIsPhysicsActive);
	}
}

