// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"

#include "Bomb.h"


ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Items/Bomb.Bomb")); //D:/Development/Demoman/Content/Items/Bomb.uasset
	if (MeshAsset.Succeeded())
	{
		BombMesh->SetStaticMesh(MeshAsset.Object);
	}

	BombMesh->SetCollisionProfileName("BlockAllDynamic");
	BombMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABomb::BlowUp()
{

	OnBlowUpBomb.Execute();
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABomb* ABomb::SpawnBomb(UWorld* World, FVector Location, int8 Power)
{
	// TODO BombManager;

	auto labdaDoGood = [](float a)
	{
		int32 temp = FMath::RoundToInt(a) % 100;
		if (temp > 50)
		{
			return a + 100 - temp;
		}
		else
		{
			return a - temp;
		}
	};
	
	Location.X = labdaDoGood(Location.X);
	Location.Y = labdaDoGood(Location.Y);

	ABomb* tempBomb = World->SpawnActor<ABomb>(TSubclassOf<ABomb>(), FTransform(FRotator(0.f), Location, FVector(1.f)));
	tempBomb->Power = Power;
	return tempBomb;
}

