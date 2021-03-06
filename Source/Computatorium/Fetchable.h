// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AcceptsFetchable.h"
#include "Fetchable.generated.h"

UCLASS()
class COMPUTATORIUM_API AFetchable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFetchable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    /** Collision mesh to represent the fetchable in the level*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* HitBox;
    
    /** Static mesh to represent the fetchable in the level*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* Mesh;

	void BindToActor(AcceptsFetchable *BindingActor, USceneComponent *BindingComponent);

private:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
	AcceptsFetchable* BindingActor;
};
