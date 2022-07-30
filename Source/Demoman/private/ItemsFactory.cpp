// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemsFactory.h"

#include "AudioDeviceManager.h"
#include "Item/BasePickableItems.h"

FItemsFactory::FItemsFactory()
{
	FItemsFactory::FactoryPointer = this;
}

FItemsFactory::~FItemsFactory()
{
	delete FactoryPointer;
}

FItemsFactory* FItemsFactory::GetFactory()
{
	if (FactoryPointer) return FactoryPointer;
	else return new FItemsFactory();
}

ABasePickableItems* FItemsFactory::CreateItemInLocation(UWorld* World, FVector Location)
{	
	int32 randNum = FMath::Rand() % RegisteredItems.Num();
	return World->SpawnActor<ABasePickableItems>(RegisteredItems[randNum], FTransform(FRotator(0.f), Location, FVector(1.f)));
}

void FItemsFactory::RegisterNewItem(UClass* item)
{
	RegisteredItems.AddUnique(item);
}
