// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Demoman/Generators/BaseMapGenerator.h"

// Sets default values
ABaseMapGenerator::ABaseMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

