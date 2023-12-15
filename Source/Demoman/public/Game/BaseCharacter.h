// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


UCLASS()
class DEMOMAN_API ABaseCharacter : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UFUNCTION(Reliable, Server)
		void SpawnBomb();
		virtual void SpawnBomb_Implementation();

	UFUNCTION(Reliable, NetMulticast)
		void Death();
		virtual void Death_Implementation();

	UFUNCTION(Reliable, NetMulticast)
		void SetAutoTeamMaterial();
		void SetAutoTeamMaterial_Implementation();

	int8 GetPlacedBombs()		{ return BombsPlaced;	}
	int8 GetCountBombs()		{ return BombsCount;	}
	int8 GetBombsPower()		{ return BombsPower;	}
	float GetMovementSpeed()	{ return MovementSpeed; }

	void AddBombsCount(int8 AddNum);
	void AddBombsPower(int8 AddNum);
	void AddMovementSpeed(float AddNum);

	void UpdateGameWidget();
	void SetTeamMaterial(UMaterialInterface* newMaterial);

	//	IDamageInterface
	bool DamageActor() override;
	void DamageActorReplicated_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void RestoreBomb();

private:
	void AddBombsPlaced(int8 AddNum);

	UFUNCTION(Reliable, Client)
		void UpdateGameWidget(int8 bombs, int8 power, float speed);
		void UpdateGameWidget_Implementation(int8 bombs, int8 power, float speed);

public:
	UPROPERTY()
		class UStaticMeshComponent* PlayerModel;
	UPROPERTY()
		class UCameraComponent* PlayerCamera;


	const int8 MaxBombsCount = 5;
	const int8 MaxBombsPower = 5;
	const float MaxMovementSpeed = 600;

protected:
	int8 BombsPlaced = 0;
	int8 BombsCount = 1;
	int8 BombsPower = 2;
	float MovementSpeed = 400;

private:
	UMaterialInterface* DeadPlayerMaterial;

	UMaterialInterface* RedPlayerMaterial;
	UMaterialInterface* BluePlayerMaterial;
	UMaterialInterface* YellowPlayerMaterial;
	UMaterialInterface* GreenPlayerMaterial;

	UMaterialInterface* PinkPlayerMaterial;
	UMaterialInterface* BrownPlayerMaterial;
	UMaterialInterface* CyanPlayerMaterial;
	UMaterialInterface* PurplePlayerMaterial;

};
