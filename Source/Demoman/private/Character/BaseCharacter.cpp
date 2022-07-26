// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Bomb.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GameCollision"));
	RootComponent = PlayerCollision;
	PlayerCollision->SetCollisionProfileName("Pawn");

	PlayerModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameModel"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Cube.Cube"));
	if (MeshAsset.Succeeded())
	{
		PlayerModel->SetStaticMesh(MeshAsset.Object);
	}
	

	PlayerModel->SetupAttachment(PlayerCollision);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	PlayerCamera->SetupAttachment(PlayerCollision);
	PlayerCamera->SetRelativeLocationAndRotation(FVector(-50.f, 0.f, 620.f), FRotator(-85.f, 0.f,0.f));

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

void ABaseCharacter::Death()
{
	PlayerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorScale3D(FVector(0.8f));
	DisableInput(Cast<APlayerController>(GetController()));
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

void ABaseCharacter::DamageActor(bool& bIsPenetrated)
{
	bIsPenetrated = true;
	Death();
}
