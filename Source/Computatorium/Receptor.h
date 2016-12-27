// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Receptor.generated.h"

UCLASS()
class COMPUTATORIUM_API AReceptor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceptor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    /** Collision mesh to represent the fetchable in the level*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Receptor", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* HitBox;

	UFUNCTION()
	bool CanAcceptFetchable(class AFetchable *Fetchable);
};
