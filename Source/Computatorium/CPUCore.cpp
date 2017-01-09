// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "ReceptorComponent.h"
#include "CPUCore.h"


// Sets default values
ACPUCore::ACPUCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the hitbox component
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = HitBox;

	// Create the static mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Create operands
	OperandOneReceptor = CreateDefaultSubobject<UReceptorComponent>(TEXT("Operand1"));
	OperandOneReceptor->SetupAttachment(RootComponent);

	OperandTwoReceptor = CreateDefaultSubobject<UReceptorComponent>(TEXT("Operand2"));
	OperandTwoReceptor->SetupAttachment(RootComponent);
	
	// Create result
    ResultReceptor = CreateDefaultSubobject<UReceptorComponent>(TEXT("Result"));
    ResultReceptor->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ACPUCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPUCore::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

