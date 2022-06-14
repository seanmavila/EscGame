// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESC_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetShouldMove(bool bShouldMove);

	void SetShouldOpen();

	void Unlock();

	bool IsLocked() const;

	FName GetUnlockTag() const;

private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	FVector OriginalLocation;

	UPROPERTY(EditAnywhere)
	FName UnlockTag;

	UPROPERTY(EditAnywhere)
	float MoveTime = 4.f;

	bool ShouldMove = false;

	bool bIsOpen = false;

	UPROPERTY(EditAnywhere)
	bool bIsLocked = true;
};
