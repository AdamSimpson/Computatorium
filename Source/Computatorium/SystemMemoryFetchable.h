// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fetchable.h"
#include "SystemMemoryFetchable.generated.h"

/**
 * 
 */
UCLASS()
class COMPUTATORIUM_API ASystemMemoryFetchable : public AFetchable
{
	GENERATED_BODY()

public:
	ASystemMemoryFetchable();

	int GetValue();
	void SetValue(int NewValue);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory", meta = (AllowPrivateAccess = "true"))
	int value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory", meta = (AllowPrivateAccess = "true"))
	int location;
};
