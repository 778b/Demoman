// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABasePickableItems;

class DEMOMAN_API FItemsFactory
{
private:
	FItemsFactory();
	~FItemsFactory();
	void operator= (FItemsFactory& ref) = delete;

public:
	static FItemsFactory* GetFactory();

	ABasePickableItems* CreateItemInLocation(UWorld* World, FVector Location);

	void RegisterNewItem(UClass* item);

private:
	static FItemsFactory* FactoryPointer;
	TArray<UClass*> RegisteredItems;
};

FItemsFactory* FItemsFactory::FactoryPointer = nullptr;