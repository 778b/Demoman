// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.generated.h"

UCLASS()
class DEMOMAN_API ABaseCharacter : public APawn
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

public:
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* PlayerCollision;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* PlayerModel;
	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* PlayerCamera;

	class UFloatingPawnMovement* PlayerMovement;


	const int8 MaxBombsCount = 5;
	const int8 MaxBombsPower = 8;
	const float MaxMovementSpeed = 500;

private:
	int8 BombsPlaced = 0;
	int8 BombsCount = 0;
	int8 BombsPower = 3;
	float MovementSpeed = 100;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void RestoreBomb();
};
