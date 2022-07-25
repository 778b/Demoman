// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Character/public/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class DEMOMAN_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();

public:


private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
