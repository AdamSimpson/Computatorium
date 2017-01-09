// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "Fetchable.h"
#include "ReceptorComponent.h"

// Sets default values for this component's properties
UReceptorComponent::UReceptorComponent()
{
	// We must give each HitBox and Mesh a uninque string name
	// otherwise when editing the blueprint the component hierarchy is jacked
	static int32 NamingHack = 1;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Create the hitbox component
	FString HitBoxString("ReceptorHitBox");
	HitBoxString.AppendInt(NamingHack);
	FName HitBoxName = FName(*HitBoxString);
	HitBox = CreateDefaultSubobject<UBoxComponent>(HitBoxName);
	HitBox->AttachTo(this);

	// Create the static mesh component
	FString MeshString("ReceptorMesh");
	MeshString.AppendInt(NamingHack);
	FName MeshName = FName(*MeshString);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(MeshName);
	Mesh->SetupAttachment(HitBox);

	NamingHack++;
}

// Called when the game starts
void UReceptorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Look for a fetchable set from the editor and correctly bind it
//	if (BoundFetchable != nullptr) {
//		BoundFetchable->BindToActor(this, Mesh);
//	}
}

USceneComponent* UReceptorComponent::GetBindingComponent() {
	return Mesh;
}

void UReceptorComponent::BindFetchable(class AFetchable *Fetchable) {
	Fetchable->BindToActor(this, Mesh);
}

void UReceptorComponent::PostBindFetchable(AFetchable *Fetchable) {
	BoundFetchable = Fetchable;

	// Hide receptor and turn off collision
	SetHiddenInGame(true, true);
//	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UReceptorComponent::PostUnbindFetchable(AFetchable *Fetchable) {
	BoundFetchable = nullptr;

	// Make receptor visible and enable collision
	SetHiddenInGame(false, false);
//	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Accept any fetchable
bool UReceptorComponent::CanBindFetchable(AFetchable *Fetchable) {
	return (Fetchable != nullptr && BoundFetchable == nullptr);
}

AFetchable* UReceptorComponent::GetBoundFetchable() {
	return BoundFetchable;
}

