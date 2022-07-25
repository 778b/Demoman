// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

DECLARE_DYNAMIC_DELEGATE(FBombDemolitionDelegate);

UCLASS()
class DEMOMAN_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();

	void BlowUp();

public:
	class UStaticMeshComponent* BombMesh;

	FBombDemolitionDelegate OnBlowUpBomb;

protected:
	virtual void BeginPlay() override;

private:
	int8 Power = 3;

public:	
	virtual void Tick(float DeltaTime) override;

	static ABomb* SpawnBomb(UWorld* World, FVector Location, int8 Power = 0);
};
