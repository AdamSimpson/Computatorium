// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "AcceptsFetchable.h"
#include "ReceptorComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMPUTATORIUM_API UReceptorComponent : public USceneComponent, public AcceptsFetchable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReceptorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Accept fetchable overrides
	USceneComponent* GetBindingComponent() override;
	void BindFetchable(AFetchable *Fetchable) override;
	bool CanBindFetchable(AFetchable *Fetchable) override;
	void PostBindFetchable(AFetchable *Fetchable) override;
	void PostUnbindFetchable(AFetchable *Fetchable) override;

	AFetchable* GetBoundFetchable();

//private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
	class AFetchable* BoundFetchable;

	/** Collision mesh to represent the fetchable in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Receptor", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HitBox;

	/** Static mesh used to setup attachment sockets */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fetchable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
};
