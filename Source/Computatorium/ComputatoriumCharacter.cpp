// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Computatorium.h"
#include "ComputatoriumCharacter.h"
#include "Fetchable.h"
#include "Receptor.h"
#include "PlayerButton.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

// Print to screen debug
// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target fetchable!"));
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

AComputatoriumCharacter::AComputatoriumCharacter() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; 
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
    
	// Register OnHit delegate
//   OnActorHit.AddDynamic(this, &AComputatoriumCharacter::OnHit);
	// Register OnOverlap delegates
//	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AComputatoriumCharacter::OnBeginOverlap);
//	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AComputatoriumCharacter::OnEndOverlap);
}

void AComputatoriumCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params;
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	// Check if player needs to bind to fetchable object
	if (TargetFetchable != nullptr) {
		GetOverlappingActors(OverlappingFetchables, AFetchable::StaticClass());

		// Loop through overlapping fetchables and check if any are our target fetchable
		for(auto OverlappingFetchable : OverlappingFetchables) {
			if (OverlappingFetchable == TargetFetchable && CanBindFetchable(TargetFetchable)) {
				this->BindFetchable(TargetFetchable);
			}
		}
	}

	// Check if player needs to deposit the fetchable object to receptor
	if (TargetReceptor != nullptr) {
	    GetOverlappingActors(OverlappingReceptors, AReceptor::StaticClass());

		// Loop through overlapping receptors and check if any are our target receptor
		for(auto OverlappingReceptor : OverlappingReceptors) {
			if (OverlappingReceptor == TargetReceptor && TargetReceptor->CanBindFetchable(BoundFetchable)) {
				TargetReceptor->BindFetchable(BoundFetchable);
			}
		}
	}

	// Check if player needs to click any buttons
	if (TargetButton != nullptr) {
		GetOverlappingActors(OverlappingButtons, APlayerButton::StaticClass());

		// Loop through overlapping buttons and check if any are our target button
		for (auto OverlappingButton : OverlappingButtons) {
			if (OverlappingButton == TargetButton) {
				TargetButton->Click();
				TargetButton = nullptr;
			}
		}
	}
}

void AComputatoriumCharacter::BindFetchable(AFetchable *Fetchable) {
	Fetchable->BindToActor(this, GetMesh());
}

bool AComputatoriumCharacter::CanBindFetchable(AFetchable* Fetchable) {
	// If we aren't holding a fetchable we can pick one up
	return (BoundFetchable == nullptr);
}

void AComputatoriumCharacter::PostUnbindFetchable(AFetchable *Fetchable) {
	// Enable navigation affect
	Fetchable->HitBox->SetCanEverAffectNavigation(true);

	// Stop player reacting to navigation affect being enabled
	this->GetController()->StopMovement();

	// Unset bound fetchable
	BoundFetchable = nullptr;
	// Unset target receptor
	TargetReceptor = nullptr;
}

void AComputatoriumCharacter::PostBindFetchable(AFetchable *Fetchable) {
	// Set the players bound fetchable
	BoundFetchable = Fetchable;
	// Unset target fetchable
	TargetFetchable = nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fetched!"));
}

USceneComponent* AComputatoriumCharacter::GetBindingComponent() {
	return GetMesh();
}

// TODO refactor this mess into more sensible functions
void AComputatoriumCharacter::SetTargetFetchable(AFetchable* Fetchable) {
	// Can only grab one fetchable at a time
	if (BoundFetchable != nullptr)
		return;

	// We have a new target fetchable
	if (Fetchable != nullptr && Fetchable != TargetFetchable) {
        // Disable navigation on old target fetcahble
		if (TargetFetchable) {
		  TargetFetchable->HitBox->SetCanEverAffectNavigation(true);
		}

		// Set the players target fetchable to the clicked on fetchable
		TargetFetchable = Fetchable;

		// Disable navigation effects from fetchable so our player can hit it
		Fetchable->HitBox->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target fetchable!"));
	}
	// Our target fetchable has been unselected
	else if(Fetchable == nullptr && TargetFetchable != nullptr) {
		TargetFetchable->HitBox->SetCanEverAffectNavigation(true);
		TargetFetchable = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unsetting target fetchable!"));
	}
}

void AComputatoriumCharacter::SetTargetReceptor(AReceptor* Receptor) {
    TargetReceptor = Receptor;
    
    if(Receptor != nullptr)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target receptor!"));
}

void AComputatoriumCharacter::SetTargetButton(APlayerButton* Button) {
	TargetButton = Button;

	if (Button != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target button!"));
}
