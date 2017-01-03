// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerButton.generated.h"

UCLASS()
class COMPUTATORIUM_API APlayerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Collision mesh to represent the fetchable in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HitBox;
	/** Static mesh used to setup attachment sockets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	virtual void Click() {};
};
