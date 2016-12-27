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
    
	// Register OnHit delegate
 //   OnActorHit.AddDynamic(this, &AComputatoriumCharacter::OnHit);

	// Register OnOverlap delegates
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AComputatoriumCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AComputatoriumCharacter::OnEndOverlap);

	OverlappingReceptor = nullptr;
	OverlappingFetchable = nullptr;
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

	// Check if player needs to pickup or dropoff fetchable
	if (TargetFetchable != nullptr && OverlappingFetchable == TargetFetchable && CanPickupFetchable(TargetFetchable)) {
		PickupFetchable(TargetFetchable);
	}
	if (TargetReceptor != nullptr && OverlappingReceptor == TargetReceptor && TargetReceptor->CanAcceptFetchable(HeldFetchable)) {
		DropOffFetchable(HeldFetchable, TargetReceptor);
	}
}

bool AComputatoriumCharacter::CanPickupFetchable(AFetchable* Fetchable) {
	// If we aren't holding a fetchable we can pick one up
	return (HeldFetchable == nullptr);
}

void AComputatoriumCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {

}

void AComputatoriumCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	                                         AActor* OtherActor, 
	                                         UPrimitiveComponent* OtherComp, 
	                                         int32 OtherBodyIndex, 
	                                         bool bFromSweep,
	                                         const FHitResult& SweepResult)  {

	if ( (OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) ) {

		// Begin to overlap a fetchable
		AFetchable *TestFetchable = Cast<AFetchable>(OtherActor);
		if (TestFetchable) {
			OverlappingFetchable = TestFetchable;
		}

		// Begin to overlap a receptor
		AReceptor *TestReceptor = Cast<AReceptor>(OtherActor);
		if (TestReceptor) {
			OverlappingReceptor = TestReceptor;
		}
	}
}

void AComputatoriumCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp,
	                                       AActor* OtherActor,
	                                       UPrimitiveComponent* OtherComp,
	                                       int32 OtherBodyIndex) {

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {

		// End overlap targeted fetchable
		if (OtherActor == OverlappingFetchable && OverlappingFetchable != nullptr) {
			OverlappingFetchable = nullptr;
		}

		// End overlap of Receptor
		if (OtherActor == OverlappingReceptor && OverlappingReceptor != nullptr) {
			OverlappingReceptor = nullptr;
		}
	}
}

void AComputatoriumCharacter::PickupFetchable(AFetchable* Fetchable) {
	// Attach fetchable to player's mesh(attaching to the actor directly attaches to it's capsule)
	const FName FSocketName = TEXT("fetchable_socket");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Fetchable->AttachToComponent(GetMesh(), AttachmentRules, FSocketName);

	// Set the held fetchable
	HeldFetchable = Fetchable;

	// Reset target fetchable
	TargetFetchable = nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fetched!"));
}

void AComputatoriumCharacter::DropOffFetchable(AFetchable* Fetchable, AReceptor* Receptor) {
	// Detach fetchable from player
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Fetchable->DetachFromActor(DetachRules);

	// Enable navigation affect
	Fetchable->HitBox->SetCanEverAffectNavigation(true);

	// Attach to receptors socket
	const FName FSocketName = TEXT("fetchable_socket");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//        HeldFetchable->AttachToComponent(TargetReceptor->Mesh, AttachmentRules, FSocketName);
	// Need to detach from player and place in hitbox

	// Unset target receptor
	TargetReceptor = nullptr;

	// Unset held fetchable
	HeldFetchable = nullptr;

	// TODO : Should probably trigger navigation event to push actor out from the box here

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Receptored!"));
}

void AComputatoriumCharacter::SetTargetFetchable(AFetchable* Fetchable) {
    TargetFetchable = Fetchable;
    
	if (Fetchable) {
		// Disable navigation effects from fetchable so our player can hit it
		Fetchable->HitBox->SetCanEverAffectNavigation(false);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target fetchable!"));
	}
}

void AComputatoriumCharacter::SetTargetReceptor(AReceptor* Receptor) {
    TargetReceptor = Receptor;
    
    if(Receptor != nullptr)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Setting new target receptor!"));
}
