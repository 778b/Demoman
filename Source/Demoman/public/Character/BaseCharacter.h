// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.generated.h"

UCLASS()
class DEMOMAN_API ABaseCharacter : public APawn, public IDamageInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void SpawnBomb();

	int8 GetPlacedBombs()		{ return BombsPlaced;	}
	int8 GetCountBombs()		{ return BombsCount;	}
	int8 GetBombsPower()		{ return BombsPower;	}
	float GetMovementSpeed()	{ return MovementSpeed; }

	void AddBombsCount(int8 AddNum);
	void AddBombsPower(int8 AddNum);
	void AddMovementSpeed(float AddNum);
private:
	void AddBombsPlaced(int8 AddNum);
	void Death();

public:
	class USphereComponent* PlayerCollision;
	class UStaticMeshComponent* PlayerModel;
	class UCameraComponent* PlayerCamera;
	class UFloatingPawnMovement* PlayerMovement;


	const int8 MaxBombsCount = 5;
	const int8 MaxBombsPower = 5;
	const float MaxMovementSpeed = 700;

private:
	int8 BombsPlaced = 0;
	int8 BombsCount = 1;
	int8 BombsPower = 2;
	float MovementSpeed = 400;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void RestoreBomb();

public:
	//	IDamageInterface
	void DamageActor(bool& bIsPenetrated) override;
};
