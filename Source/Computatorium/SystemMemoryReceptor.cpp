// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "Receptor.h"
#include "SystemMemoryFetchable.h"
#include "SystemMemoryReceptor.h"

bool ASystemMemoryReceptor::HasMemory() {
	return (GetBoundFetchable() != nullptr);
}

ASystemMemoryFetchable* ASystemMemoryReceptor::GetBoundMemory() {
	return Cast<ASystemMemoryFetchable>(GetBoundFetchable());
}

int ASystemMemoryReceptor::GetMemoryValue() {
	return GetBoundMemory()->GetValue();
}

void ASystemMemoryReceptor::SetMemoryValue(int Value) {
	GetBoundMemory()->SetValue(Value);
}