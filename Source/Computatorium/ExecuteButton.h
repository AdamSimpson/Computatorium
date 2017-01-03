// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerButton.h"
#include "ExecuteButton.generated.h"

UCLASS()
class COMPUTATORIUM_API AExecuteButton : public APlayerButton
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExecuteButton();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operand", meta = (AllowPrivateAccess = "true"))
	class ASystemMemoryReceptor* OperandOneReceptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Operand", meta = (AllowPrivateAccess = "true"))
	class ASystemMemoryReceptor* OperandTwoReceptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result", meta = (AllowPrivateAccess = "true"))
	class ASystemMemoryReceptor* ResultReceptor;

	virtual void Click() override;
};
