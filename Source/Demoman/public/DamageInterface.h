// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};


class DEMOMAN_API IDamageInterface
{
	GENERATED_BODY()
public:

	virtual bool DamageActor() = 0;

	UFUNCTION(Reliable, NetMulticast)
		virtual void DamageActorReplicated();
		virtual void DamageActorReplicated_Implementation() = 0;
};
