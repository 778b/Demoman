// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMapGenerator.generated.h"

UCLASS()
class DEMOMAN_API ABaseMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseMapGenerator();

	UFUNCTION(BlueprintCallable)
		void BuildMap();

protected:
	virtual void BeginPlay() override;

};
