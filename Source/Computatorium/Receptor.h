// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AcceptsFetchable.h"
#include "Receptor.generated.h"

UCLASS()
class COMPUTATORIUM_API AReceptor : public AActor, public AcceptsFetchable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceptor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Accept fetchable overrides
	USceneComponent* GetBindingComponent() override;
	void BindFetchable(AFetchable *Fetchable) override;
	bool CanBindFetchable(AFetchable *Fetchable) override;
	void PostBindFetchable(AFetchable *Fetchable) override;
	void PostUnbindFetchable(AFetchable *Fetchable) override;

	AFetchable* GetBoundFetchable();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
	class AFetchable* BoundFetchable;

	/** Collision mesh to represent the fetchable in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Receptor", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HitBox;

	/** Static mesh used to setup attachment sockets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
};
