// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Computatorium.h"
#include "ComputatoriumCharacter.h"
#include "Fetchable.h"
#include "Receptor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

// Print to screen debug
// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target fetchable!"));
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

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
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
    
    OnActorHit.AddDynamic(this, &AComputatoriumCharacter::OnHit);
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
}

void AComputatoriumCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
    
    if(OtherActor == TargetFetchable) {
        // Disable Collision on Fetchable
        TargetFetchable->SetActorEnableCollision(false);
        
        // Attach fetchable to player's mesh(attaching to the actor directly attaches to it's capsule)
        const FName FSocketName = TEXT("fetchable_socket");
        const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        TargetFetchable->AttachToComponent(GetMesh(), AttachmentRules, FSocketName);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fetched!"));
    }
    else if(OtherActor == TargetReceptor) {
        // Disable Collision on Fetchable
        TargetFetchable->SetActorEnableCollision(false);
        
        // Attach fetchable to player's mesh(attaching to the actor directly attaches to it's capsule)
        const FName FSocketName = TEXT("fetchable_socket");
        const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        TargetFetchable->AttachToComponent(TargetReceptor->GetMesh(), AttachmentRules, FSocketName);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fetched!"));
    }
}

void AComputatoriumCharacter::SetTargetFetchable(AFetchable* Fetchable) {
    TargetFetchable = Fetchable;
    
    if(Fetchable != nullptr)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target fetchable!"));
}