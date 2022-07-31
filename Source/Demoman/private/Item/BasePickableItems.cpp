// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/BasePickableItems.h"

#include "ItemsFactory.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Game/BaseCharacter.h"




ABasePickableItems::ABasePickableItems()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PrimaryActorTick.TickInterval = 0.01f;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemsMesh"));
	RootComponent = ItemMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Bonus.Bonus"));
	if (MeshAsset.Succeeded())
	{
		ItemMesh->SetStaticMesh(MeshAsset.Object);
	}
	ItemMesh->SetCollisionProfileName("OverlapAll");
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &ABasePickableItems::OnBeginOverlap);
}

void ABasePickableItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(GetActorRotation() + FRotator(0.f, 1.f, 0.f));
}

void ABasePickableItems::DamageActor(bool& bIsPenetrated)
{
	bIsPenetrated = true;
	Destroy();
}

void ABasePickableItems::SetLifeTime(int8 newTime)
{
	if (newTime > 0) LifeTime = newTime;
	else check(false);
}

void ABasePickableItems::SetMissTime(int8 newTime)
{
	if (newTime & 1) SpeedDeath = newTime;
	else check(false);
}

void ABasePickableItems::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(MissingTimer, this, &ABasePickableItems::StartMissing, 6.f, false);
	
}

void ABasePickableItems::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* tempCharacter = Cast<ABaseCharacter>(OtherActor);
	if (tempCharacter)
	{
		Consume(tempCharacter);
	}
}

void ABasePickableItems::StartMissing()
{
	GetWorld()->GetTimerManager().SetTimer(MissingTimer, this, &ABasePickableItems::TickMissing, 0.3f, false);
}

void ABasePickableItems::TickMissing()
{
	if (SpeedDeath > 0)
	{
		if (SpeedDeath & 1) ItemMesh->SetVisibility(false);
		else ItemMesh->SetVisibility(true);
		--SpeedDeath;
		GetWorld()->GetTimerManager().SetTimer(MissingTimer, this, &ABasePickableItems::TickMissing, 0.3f, false);
	}
	else Destroy();
}


