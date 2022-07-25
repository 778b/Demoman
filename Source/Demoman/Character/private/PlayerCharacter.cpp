// Fill out your copyright notice in the Description page of Project Settings.



#include "../../Character/public/PlayerCharacter.h"


//PlayerCollision;
//PlayerModel;
//PlayerCameraComponent;

APlayerCharacter::APlayerCharacter()
{

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("DoBomb", EInputEvent::IE_Pressed, this, &ABaseCharacter::SpawnBomb);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0) 
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}


