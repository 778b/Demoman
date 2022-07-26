// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBlock.generated.h"

UCLASS()
class DEMOMAN_API ABaseBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBlock();

public:
	class UStaticMeshComponent* BlockMesh;

	const float ChanceToDrop = 0;

	void DamageBlock();

private:
	int8 BlockHealth = -1;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
