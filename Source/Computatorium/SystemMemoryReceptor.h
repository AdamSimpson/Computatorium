// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Receptor.h"
#include "SystemMemoryReceptor.generated.h"

UCLASS()
class COMPUTATORIUM_API ASystemMemoryReceptor : public AReceptor
{
	GENERATED_BODY()
public:
	bool HasMemory();
	int GetMemoryValue();
	void SetMemoryValue(int Value);
private:
	class ASystemMemoryFetchable* GetBoundMemory();
};
