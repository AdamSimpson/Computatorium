// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CPUCore.generated.h"

UCLASS()
class COMPUTATORIUM_API ACPUCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPUCore();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	/** Collision mesh to represent the fetchable in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HitBox;
	/** Static mesh used to setup attachment sockets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operand", meta = (AllowPrivateAccess = "true"))
	class UReceptorComponent* OperandOneReceptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operand", meta = (AllowPrivateAccess = "true"))
	class UReceptorComponent* OperandTwoReceptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result", meta = (AllowPrivateAccess = "true"))
	class UReceptorComponent* ResultReceptor;
};
