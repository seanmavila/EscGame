// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grabber.h"
#include "InteractComponent.h"

//////////////////////////////////////////////////////////////////////////
// AEscCharacter

AEscCharacter::AEscCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "head"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a grabber component on the player
	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));
	Grabber->SetupAttachment(FollowCamera);

	//Create an interaction component on the player
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact Component"));
	InteractComponent->SetupAttachment(FollowCamera);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEscCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AEscCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AEscCharacter::MoveRight);

	// Bind Grab event
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AEscCharacter::Grab);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &AEscCharacter::Release);

	// Bind Rotation events
	PlayerInputComponent->BindAction("RotatePitch", IE_Pressed, this, &AEscCharacter::RotateGrabbedObjectPitch);
	PlayerInputComponent->BindAction("RotateYaw", IE_Pressed, this, &AEscCharacter::RotateGrabbedObjectYaw);
	PlayerInputComponent->BindAction("RotateRoll", IE_Pressed, this, &AEscCharacter::RotateGrabbedObjectRoll);

	PlayerInputComponent->BindAction("RotatePitch", IE_Released, this, &AEscCharacter::StopRotateGrabbedObject);
	PlayerInputComponent->BindAction("RotateYaw", IE_Released, this, &AEscCharacter::StopRotateGrabbedObject);
	PlayerInputComponent->BindAction("RotateRoll", IE_Released, this, &AEscCharacter::StopRotateGrabbedObject);

	// Bind Object range extention and retraction
	PlayerInputComponent->BindAction("ExtendRange", IE_Pressed, this, &AEscCharacter::ExtendRangeOfObject);
	PlayerInputComponent->BindAction("RetractRange", IE_Pressed, this, &AEscCharacter::RetractRangeOfObject);

	PlayerInputComponent->BindAction("ExtendRange", IE_Released, this, &AEscCharacter::StopObjectExtension);
	PlayerInputComponent->BindAction("RetractRange", IE_Released, this, &AEscCharacter::StopObjectExtension);

	// Bind Object interactio event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AEscCharacter::Interact);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AEscCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AEscCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AEscCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AEscCharacter::TouchStopped);
}

void AEscCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void AEscCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShoulRotatePitch)
	{
		Grabber->RotateActor(1, 0, 0);
	}
	if (bShoulRotateYaw)
	{
		Grabber->RotateActor(0, 1, 0);
	}
	if (bShoulRotateRoll)
	{
		Grabber->RotateActor(0, 0, 1);
	}
	if (bShouldExtend)
	{
		Grabber->ExtendRangeOfObject();
	}
	if (bShouldRetract)
	{
		Grabber->RetractRangeOfObject();
	}
}

void AEscCharacter::RotateGrabbedObjectPitch()
{
	bShoulRotatePitch = true;
}

void AEscCharacter::RotateGrabbedObjectYaw()
{
	bShoulRotateYaw = true;
}

void AEscCharacter::RotateGrabbedObjectRoll()
{
	bShoulRotateRoll = true;
}

void AEscCharacter::StopRotateGrabbedObject()
{
	Grabber->Grab();
	bShoulRotatePitch = false;
	bShoulRotateYaw = false;
	bShoulRotateRoll = false;
}

void AEscCharacter::ExtendRangeOfObject()
{
	bShouldExtend = true;
}

void AEscCharacter::RetractRangeOfObject()
{
	bShouldRetract = true;
}

void AEscCharacter::StopObjectExtension()
{
	bShouldExtend = false;
	bShouldRetract = false;
}

void AEscCharacter::AddItemToInventory(AActor* Item)
{
	Inventory.Add(Item);
	 
}

bool AEscCharacter::IsItemInInventory(FName ItemTag) const
{
	if (Inventory.Num() > 0)
	{
		for (AActor* Item : Inventory)
		{
			if (Item->ActorHasTag(ItemTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("Item found..."));
				return true;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Item not in inventory..."));
	return false;
}

void AEscCharacter::RemoveItemFromInventory(FName ItemTag)
{
	if (Inventory.Num() > 0)
	{
		for (AActor* Item : Inventory)
		{
			if (Item->ActorHasTag(ItemTag))
			{
				Inventory.Remove(Item);
				return;
			}
		}
	}
}

void AEscCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AEscCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AEscCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AEscCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


void AEscCharacter::Grab()
{
	Grabber->Grab();
}

void AEscCharacter::Release()
{
	Grabber->Release();
}

void AEscCharacter::Interact()
{
	InteractComponent->Interaction();
}

void AEscCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEscCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
