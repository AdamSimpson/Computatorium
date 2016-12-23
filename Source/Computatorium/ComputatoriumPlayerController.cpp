// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Computatorium.h"
#include "ComputatoriumPlayerController.h"
#include "Fetchable.h"
#include "Receptor.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "ComputatoriumCharacter.h"

AComputatoriumPlayerController::AComputatoriumPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AComputatoriumPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor) {
		MoveToMouseCursor();
	}
}

void AComputatoriumPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AComputatoriumPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AComputatoriumPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AComputatoriumPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AComputatoriumPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AComputatoriumPlayerController::OnResetVR);
}

void AComputatoriumPlayerController::OnResetVR() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AComputatoriumPlayerController::MoveToMouseCursor() {
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
		if (AComputatoriumCharacter* MyPawn = Cast<AComputatoriumCharacter>(GetPawn())) {
			if (MyPawn->GetCursorToWorld()) {
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else {
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit) {
			// We hit something, move there
			SetNewMoveDestination(Hit);
		}
	}
}

void AComputatoriumPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit) {
		// We hit something, move there and potentially update fetchable target
		SetNewMoveDestination(HitResult);
	}
}

void AComputatoriumPlayerController::SetNewMoveDestination(const FHitResult& Hit)
{
    auto Player = Cast<AComputatoriumCharacter>(GetPawn());
    
	if (Player) {
        FVector HitLocation = Hit.ImpactPoint;
        UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(HitLocation, Player->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f)) {
			NavSys->SimpleMoveToLocation(this, HitLocation);
            
            auto* HitActor = Hit.GetActor();

            // Set players target fetchable
            auto *TestFetchable = Cast<AFetchable>(HitActor);
            Player->SetTargetFetchable(TestFetchable);
            
            // Set players target receptor
            auto *TestReceptor = Cast<AReceptor>(HitActor);
            Player->SetTargetReceptor(TestReceptor);
		}
	}
}

void AComputatoriumPlayerController::OnSetDestinationPressed() {
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AComputatoriumPlayerController::OnSetDestinationReleased() {
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
