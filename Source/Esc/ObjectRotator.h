// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectRotator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESC_API UObjectRotator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectRotator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetShouldOpen();

	void Unlock();

	bool IsLocked() const;

	FName GetUnlockTag() const;

private:
	UPROPERTY(EditAnywhere)
	float PitchOffset;

	UPROPERTY(EditAnywhere)
	float RollOffset;

	UPROPERTY(EditAnywhere)
	float YawOffset;

	UPROPERTY(EditAnywhere)
	FRotator OriginalRotation;

	UPROPERTY(EditAnywhere)
	FName UnlockTag;

	UPROPERTY(EditAnywhere)
	float RotateTime = 4.f;

	bool bIsOpen = false;

	UPROPERTY(EditAnywhere)
	bool bIsLocked = true;

		
};
