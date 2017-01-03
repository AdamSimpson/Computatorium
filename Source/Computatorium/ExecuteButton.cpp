// Fill out your copyright notice in the Description page of Project Settings.

#include "Computatorium.h"
#include "SystemMemoryReceptor.h"
#include "ExecuteButton.h"

// runtime debug printing
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
AExecuteButton::AExecuteButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AExecuteButton::Click() {
	int value = OperandOneReceptor->GetMemoryValue() + OperandTwoReceptor->GetMemoryValue();
	ResultReceptor->SetMemoryValue(value);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pressed Execute Button!"));
}

