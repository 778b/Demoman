// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BaseCharacter.h"

#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Player/GamePlayerState.h"
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

	ConstructorHelpers::FObjectFinder<UMaterialInterface> DeadMaterialAsset(TEXT("/Game/Material/Skin/M_DeadedPlayer.M_DeadedPlayer"));
	if (DeadMaterialAsset.Succeeded())
	{
		DeadPlayerMaterial = DeadMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BlueMaterialAsset(TEXT("/Game/Material/Skin/M_BluePlayer.M_BluePlayer"));
	if (BlueMaterialAsset.Succeeded())
	{
		BluePlayerMaterial = BlueMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> RedMaterialAsset(TEXT("/Game/Material/Skin/M_RedPlayer.M_RedPlayer"));
	if (RedMaterialAsset.Succeeded())
	{
		RedPlayerMaterial = RedMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> GreenMaterialAsset(TEXT("/Game/Material/Skin/M_GreenPlayer.M_GreenPlayer"));
	if (GreenMaterialAsset.Succeeded())
	{
		GreenPlayerMaterial = GreenMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> YellowMaterialAsset(TEXT("/Game/Material/Skin/M_YellowPlayer.M_YellowPlayer"));
	if (YellowMaterialAsset.Succeeded())
	{
		YellowPlayerMaterial = YellowMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> PinkMaterialAsset(TEXT("/Game/Material/Skin/M_PinkPlayer.M_PinkPlayer"));
	if (YellowMaterialAsset.Succeeded())
	{
		PinkPlayerMaterial = PinkMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BrownMaterialAsset(TEXT("/Game/Material/Skin/M_BrownPlayer.M_BrownPlayer"));
	if (BrownMaterialAsset.Succeeded())
	{
		BrownPlayerMaterial = BrownMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> CyanMaterialAsset(TEXT("/Game/Material/Skin/M_CyanPlayer.M_CyanPlayer"));
	if (CyanMaterialAsset.Succeeded())
	{
		CyanPlayerMaterial = CyanMaterialAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> PurpleMaterialAsset(TEXT("/Game/Material/Skin/M_PurplePlayer.M_PurplePlayer"));
	if (PurpleMaterialAsset.Succeeded())
	{
		PurplePlayerMaterial = PurpleMaterialAsset.Object;
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

void ABaseCharacter::SetAutoTeamMaterial_Implementation()
{
	AGamePlayerState* OurState = GetPlayerState<AGamePlayerState>();
	switch (OurState->GetPlayerLobbyState())
	{
	case EPlayerLobbyTeam::Red:
		PlayerModel->SetMaterial(0, RedPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Blue:
		PlayerModel->SetMaterial(0, BluePlayerMaterial);
		return;
	case EPlayerLobbyTeam::Yellow:
		PlayerModel->SetMaterial(0, YellowPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Green:
		PlayerModel->SetMaterial(0, GreenPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Pink:
		PlayerModel->SetMaterial(0, PinkPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Cyan:
		PlayerModel->SetMaterial(0, CyanPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Brown:
		PlayerModel->SetMaterial(0, BrownPlayerMaterial);
		return;
	case EPlayerLobbyTeam::Purple:
		PlayerModel->SetMaterial(0, PurplePlayerMaterial);
		return;
	default:
		checkf(true, TEXT("How u start game without selecting player state?"));
	}
	
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

