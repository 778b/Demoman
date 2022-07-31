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
	UFUNCTION(Reliable, Server)
		void Death();
	virtual void Death_Implementation();

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
	UPROPERTY()
		class UStaticMeshComponent* PlayerModel;
	UPROPERTY()
		class UCameraComponent* PlayerCamera;


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
