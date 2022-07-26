// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"

#include "Player/GamePlayerController.h"
#include "Components/InputComponent.h"


APlayerCharacter::APlayerCharacter()
{

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AGamePlayerController* tempController = Cast<AGamePlayerController>(GetController());
	checkf(tempController, TEXT("PlayerCharacter missed controller"));
	//tempController->UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
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
		AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
	}
}


