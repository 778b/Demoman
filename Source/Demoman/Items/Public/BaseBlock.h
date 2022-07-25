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
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* BlockMesh;
	UPROPERTY(EditDefaultsOnly)
		int8 BlockHealth = -1	UMETA(ClampMin(-1), ClampMax(10));
	UPROPERTY(EditDefaultsOnly)
		float ChanceToDrop = 0	UMETA(ClampMin(0), ClampMax(100));

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
