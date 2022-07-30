// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Actor.h"
#include "BasePickableItems.generated.h"


UCLASS(Abstract)
class DEMOMAN_API ABasePickableItems : public AActor, public IDamageInterface
{
	GENERATED_BODY()
	
public:	
	ABasePickableItems();

	virtual void Consume(class ABaseCharacter* InstigatorChar) PURE_VIRTUAL(ABasePickableItems::Consume, );

	void Tick(float DeltaTime) override;

	//	IDamageInterface
	void DamageActor(bool& bIsPenetrated) override;

public:
	class UStaticMeshComponent* ItemMesh;

	void SetLifeTime(int8 newTime);
	void SetMissTime(int8 newTime);

	int8 GetLifeTime() { return LifeTime;	};
	int8 GetMissTime() { return SpeedDeath; };

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FTimerHandle MissingTimer;

	int8 LifeTime = 10;		// Any number, Time before start missing;
	int8 SpeedDeath = 15;	// Must be %1 for best view 

private:
	UFUNCTION()
		void StartMissing();
	UFUNCTION()
		void TickMissing();
};
