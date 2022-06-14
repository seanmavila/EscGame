// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractComponent.generated.h"

class UMover;
class UObjectRotator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESC_API UInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interaction();

private:

	bool LookForInteraction(FHitResult& OutHitResult) const;

	UPROPERTY(EditAnywhere)
	float MaxInteractDistance = 400;

	UPROPERTY(EditAnywhere)
	float InteractRadius = 25;
	
	bool HasHit;

	FHitResult HitResult;

	UMover* Mover;

	UObjectRotator* ObjectRotator;

		
};
