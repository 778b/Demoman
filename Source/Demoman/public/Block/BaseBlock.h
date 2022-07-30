// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Actor.h"
#include "BaseBlock.generated.h"

UCLASS()
class DEMOMAN_API ABaseBlock : public AActor, public IDamageInterface
{
	GENERATED_BODY()
	
public:	
	ABaseBlock();

public:
	class UStaticMeshComponent* BlockMesh;

	float GetChanceToDrop() { return ChanceToDrop; };
	void SetChanceToDrop(float newChance);

public:

	void DropItem();

	//	IDamageInterface
	void DamageActor(bool& bIsPenetrated) override;

private:
	float ChanceToDrop = 0;					// 0.f - 100.f

protected:
	virtual void BeginPlay() override;
	int8 BlockHealth = -1;
};
