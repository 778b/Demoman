// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BaseCharacter.h"

#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	PlayerModel->SetupAttachment(RootComponent);
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocationAndRotation(FVector(-50.f, 0.f, 620.f), FRotator(-85.f, 0.f,0.f));

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->MaxAcceleration = 2000;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Material/Skin/M_DeadedPlayer.M_DeadedPlayer"));
	if (MaterialAsset.Succeeded())
	{
		DeadPlayerMaterial = MaterialAsset.Object;
	}
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
	AGamePlayerController* tempController = Cast<AGamePlayerController>(GetController());
	if (tempController)
	{
		tempController->UpdateGameWidget(bombs,power,speed);
	}
}

void ABaseCharacter::Death_Implementation()
{
	FCollisionResponseContainer ECCContrainer;
	ECCContrainer.SetResponse(ECC_Pawn, ECR_Ignore);
	ECCContrainer.SetResponse(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(ECCContrainer);

	SetActorScale3D(FVector(1.05f));

	PlayerModel->SetMaterial(0, DeadPlayerMaterial);

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

void ABaseCharacter::UpdateGameWidget()
{
	UpdateGameWidget(BombsCount, BombsPower, MovementSpeed);
}
