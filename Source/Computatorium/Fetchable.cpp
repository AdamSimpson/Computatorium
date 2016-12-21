// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "Fetchable.h"

// Fetchable items can be picked up by a worker character and moved

// Sets default values
AFetchable::AFetchable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    // Create the static mesh component
    FetchableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FetchableMesh"));
    RootComponent = FetchableMesh;
}

// Called when the game starts or when spawned
void AFetchable::BeginPlay()
{
	Super::BeginPlay();
	
}
