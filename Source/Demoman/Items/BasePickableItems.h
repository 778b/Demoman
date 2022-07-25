// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickableItems.generated.h"


UCLASS(Abstract)
class DEMOMAN_API ABasePickableItems : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickableItems();

	virtual void Consume(class BaseCharacter* InstigatorChar) PURE_VIRTUAL(ABasePickableItems::Consume, );

public:
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* ItemMesh;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	virtual void Tick(float DeltaTime) override;

};
