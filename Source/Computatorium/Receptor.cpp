// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "Receptor.h"
#include "Fetchable.h"

// Sets default values
AReceptor::AReceptor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create the hitbox component
    HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    RootComponent = HitBox;

	// Create the static mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AReceptor::BeginPlay() {
	Super::BeginPlay();

	// Look for a fetchable set from the editor and correctly bind it
	if (BoundFetchable != nullptr) {
		BoundFetchable->BindToActor(this, Mesh);
	}
}

USceneComponent* AReceptor::GetBindingComponent() {
	return Mesh;
}

void AReceptor::BindFetchable(class AFetchable *Fetchable) {
	Fetchable->BindToActor(this, Mesh);
}

void AReceptor::PostBindFetchable(AFetchable *Fetchable) {
	BoundFetchable = Fetchable;

	// Hide receptor and turn off collision
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AReceptor::PostUnbindFetchable(AFetchable *Fetchable) {
	BoundFetchable = nullptr;

	// Make receptor visible and enable collision
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

// Accept any fetchable
bool AReceptor::CanBindFetchable(AFetchable *Fetchable) {
	return (Fetchable != nullptr && BoundFetchable == nullptr);
}

AFetchable* AReceptor::GetBoundFetchable() {
	return BoundFetchable;
}
