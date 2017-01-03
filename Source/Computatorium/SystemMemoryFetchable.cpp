// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "SystemMemoryFetchable.h"

ASystemMemoryFetchable::ASystemMemoryFetchable() {}

int ASystemMemoryFetchable::GetValue() {
	return value;
}

void ASystemMemoryFetchable::SetValue(int NewValue) {
	value = NewValue;
}

