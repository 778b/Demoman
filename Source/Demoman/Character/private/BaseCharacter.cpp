// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Character/public/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../../Items/public/Bomb.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GameCollision"));
	RootComponent = PlayerCollision;
	PlayerModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameModel"));
	PlayerModel->SetupAttachment(PlayerCollision);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	PlayerCamera->SetupAttachment(PlayerCollision);

	PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PlayerMovement"));
	PlayerMovement->MaxSpeed = MovementSpeed;
}

void ABaseCharacter::AddBombsCount(int8 AddNum)
{
	if (AddNum + GetCountBombs() <= MaxBombsCount)
	{
		BombsCount += AddNum;
	}
}

void ABaseCharacter::AddBombsPlaced(int8 AddNum)
{
	if (BombsCount + AddNum < 0) check(true);
	BombsPlaced += AddNum;
}

void ABaseCharacter::AddBombsPower(int8 AddNum)
{
	if (AddNum + BombsPower <= MaxBombsPower)
	{
		BombsPower += AddNum;
	}
}

void ABaseCharacter::AddMovementSpeed(float AddNum)
{
	if (AddNum + MovementSpeed <= MaxMovementSpeed)
	{
		MovementSpeed += AddNum;
	}
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::SpawnBomb()
{
	if (GetCountBombs() - GetPlacedBombs() > 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Bomb Placed");

		AddBombsPlaced(1);

		ABomb* tempBomb = ABomb::SpawnBomb(GetWorld(), GetActorLocation(), GetBombsPower());
		tempBomb->OnBlowUpBomb.BindUFunction(this, "RestoreBomb");
	}
	else if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Bomb Unplaced");
}

void ABaseCharacter::RestoreBomb()
{
	AddBombsPlaced(-1);
}
