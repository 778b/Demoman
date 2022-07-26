// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

DECLARE_DYNAMIC_DELEGATE(FBombDemolitionDelegate);

UCLASS()
class DEMOMAN_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();

	UFUNCTION()
		void BlowUp();

public:
	class UStaticMeshComponent* BombMesh;

	FBombDemolitionDelegate OnBlowUpBomb;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	int8 Power = 2;

	FTimerHandle TimerHandler;

public:	
	static ABomb* SpawnBomb(UWorld* World, FVector Location, int8 Power = 0);

private:
	void DamageInDirection(FVector Direction, TArray<AActor*>& ignoreActorsAndSelf);

};
