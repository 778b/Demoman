// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BaseCharacter.h"

#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Player/PlayerHUD.h"
#include "Player/GamePlayerController.h"
#include "Bomb.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	JumpMaxCount = 0;
	bReplicates = true;
	 

	PlayerModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameModel"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Cube.Cube"));
	if (MeshAsset.Succeeded())
	{
		PlayerModel->SetStaticMesh(MeshAsset.Object);
	}
	
	GetCapsuleComponent()->SetCapsuleSize(35.f, 35.f);
	PlayerModel->SetupAttachment(RootComponent);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocationAndRotation(FVector(-50.f, 0.f, 620.f), FRotator(-85.f, 0.f,0.f));

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->MaxAcceleration = 2000;
}

void ABaseCharacter::AddBombsCount(int8 AddNum)
{
	if (AddNum + GetCountBombs() <= MaxBombsCount)
	{
		BombsCount += AddNum;
	}
	UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
}

void ABaseCharacter::AddBombsPlaced(int8 AddNum)
{
	if (BombsCount + AddNum < 0) check(true);
	BombsPlaced += AddNum;
}

void ABaseCharacter::UpdateGameWidget_Implementation(int8 bombs, int8 power, float speed)
{
	auto tempController = Cast<AGamePlayerController>(GetController());
	if (tempController)
	{
		tempController->UpdateGameWidget(bombs,power,speed);
	}
}

void ABaseCharacter::Death_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshAsset(TEXT("/Game/Material/Skin/MI_DeadedPlayer.MI_DeadedPlayer"));
	if (MeshAsset.Succeeded())
	{
		PlayerModel->SetMaterial(0, MeshAsset.Object);
	}
	DisableInput(Cast<APlayerController>(GetController()));
}

void ABaseCharacter::AddBombsPower(int8 AddNum)
{
	if (AddNum + BombsPower <= MaxBombsPower)
	{
		BombsPower += AddNum;
	}
	UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
}

void ABaseCharacter::AddMovementSpeed(float AddNum)
{
	if (AddNum + MovementSpeed <= MaxMovementSpeed)
	{
		MovementSpeed += AddNum;
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
	UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
}

void ABaseCharacter::SpawnBomb_Implementation()
{
	if (GetCountBombs() - GetPlacedBombs() > 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Bomb Placed");

		AddBombsPlaced(1);

		ABomb* tempBomb = ABomb::SpawnBomb(GetWorld(), GetActorLocation(), GetBombsPower());
		tempBomb->OnBlowUpBomb.BindUFunction(this, "RestoreBomb");
	}
	//else if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Bomb Unplaced");
}

void ABaseCharacter::RestoreBomb()
{
	AddBombsPlaced(-1);
}

bool ABaseCharacter::DamageActor()
{
	
	Execute_DamageActorReplicated(this);
	return true;
}

void ABaseCharacter::DamageActorReplicated_Implementation()
{
	Death();
}
