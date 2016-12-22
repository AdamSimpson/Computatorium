// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ComputatoriumCharacter.generated.h"

class AFetchable;
class AReceptor;

UCLASS(Blueprintable)
class AComputatoriumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AComputatoriumCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
    
    UFUNCTION(BlueprintCallable, Category="Fetchable")
    void SetTargetFetchable(AFetchable* fetchable);
    
    UFUNCTION(BlueprintCallable, Category="Receptor")
    void SetTargetReseptor(AReceptor* receptor);
    
    UFUNCTION()
    void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
    
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
    
    UPROPERTY(VisibleAnywhere, Category="Fetchable")
    AFetchable* TargetFetchable;
    
    UPROPERTY(VisibleAnywhere, Category="Receptor")
    AReceptor* TargetReceptor;
};

