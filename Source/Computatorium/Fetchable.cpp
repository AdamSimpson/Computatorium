// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "AcceptsFetchable.h"
#include "Fetchable.h"

// Fetchable items can be picked up by a worker character and moved

// Sets default values
AFetchable::AFetchable() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    // Create the hitbox component
    HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    RootComponent = HitBox;
    
    // Create the static mesh component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

	BindingActor = nullptr;
}

// Assume Component contains a socket named "fetchable_socket"
void AFetchable::BindToActor(AcceptsFetchable *Actor, USceneComponent *Component) {
	// Detach from current binding actor
	if (BindingActor != nullptr) {
		BindingActor->PreUnbindFetchable(this);

		const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		DetachFromActor(DetachRules);

		BindingActor->PostUnbindFetchable(this);
	}

	// Set new binding actor
	BindingActor = Actor;

	// Bind to Actor
	BindingActor->PreBindFetchable(this);

	// Attach fetchable to mesh socket
	const FName FSocketName = TEXT("fetchable_socket");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,
		                                            EAttachmentRule::SnapToTarget,
		                                            EAttachmentRule::KeepWorld,
		                                            true);
	AttachToComponent(Component, AttachmentRules, FSocketName);

	BindingActor->PostBindFetchable(this);
}

// Called when the game starts or when spawned
void AFetchable::BeginPlay() {
	Super::BeginPlay();	
}
