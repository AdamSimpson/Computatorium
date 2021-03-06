// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "PlayerButton.h"


// Sets default values
APlayerButton::APlayerButton() {
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
void APlayerButton::BeginPlay() {
	Super::BeginPlay();
}

