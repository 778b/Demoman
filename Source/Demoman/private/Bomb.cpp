// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"

#include "DrawDebugHelpers.h"
#include "DamageInterface.h"
#include "Components/StaticMeshComponent.h"




ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/Bomb.Bomb"));
	if (MeshAsset.Succeeded())
	{
		BombMesh->SetStaticMesh(MeshAsset.Object);
	}

	BombMesh->SetCollisionProfileName("OverlapAll");
	BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BombMesh->OnComponentEndOverlap.AddDynamic(this, &ABomb::OnEndOverlap);
	BombMesh->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnBeginOverlap);

	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ABomb::BlowUp, 4.f);


}


void ABomb::BlowUp()
{
	OnBlowUpBomb.Execute();
	TArray<AActor*> IgnoreActors = { this };
	DamageInDirection(FVector(1.f, 0.f, 0.f),	IgnoreActors);
	DamageInDirection(FVector(-1.f, 0.f, 0.f),	IgnoreActors);
	DamageInDirection(FVector(0.f, 1.f, 0.f),	IgnoreActors);
	DamageInDirection(FVector(0.f, -1.f, 0.f),	IgnoreActors);
	Destroy();
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABomb::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TSet<AActor*> tempArray;
	BombMesh->GetOverlappingActors(tempArray);
	if (tempArray.Num() == 0) BombMesh->SetCollisionProfileName("BlockAllDynamic");
	return;
}

void ABomb::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	return;
}


ABomb* ABomb::SpawnBomb(UWorld* World, FVector Location, int8 Power)
{
	// TODO BombManager; but Unreal already have a his own manager :)

	auto labdaDoGood = [](float a)
	{
		int32 temp = FMath::RoundToInt(a) % 100;
		if (a > 0)
		{
			if (temp > 50) return a + 100 - temp;	// 1260 + 100 - 60 = 1300
			else return a - temp;					// 1230 - 30 = 1200
		}
		else
		{
			if (temp < -50) return a - 100 - temp;	// -1480 - 100 + 80 = -1500
			else return a - temp;					// -1320 + 20 = -1300
		}
	};
	
	Location.X = labdaDoGood(Location.X);
	Location.Y = labdaDoGood(Location.Y);

	ABomb* tempBomb = World->SpawnActor<ABomb>(ABomb::StaticClass(), FTransform(FRotator(0.f), Location, FVector(1.f)));
	check(tempBomb);

	tempBomb->Power = Power;
	return tempBomb;
}

void ABomb::DamageInDirection(FVector Direction, TArray<AActor*>& ignoreActorsAndSelf)
{
	FHitResult hit;
	FVector endTrace = (Power * 100 * Direction) + GetActorLocation();
	FCollisionQueryParams newParam;
	newParam.AddIgnoredActors(ignoreActorsAndSelf);
	bool isHitted = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), endTrace, ECollisionChannel::ECC_Visibility, newParam);

	// DrawDebugLine(GetWorld(), GetActorLocation(), endTrace, FColor::Emerald, false, 3.5f, 0U, 2.f);
	if (isHitted)
	{
		// DrawDebugPoint(GetWorld(), hit.Location, 25.f, FColor::Red, false, 3.5f);

		bool bIsPenetrated = false;
		IDamageInterface* tempActor = Cast<IDamageInterface>(hit.Actor.Get());
		if (tempActor)
		{
			tempActor->DamageActor(bIsPenetrated);
			if (bIsPenetrated)
			{
				ignoreActorsAndSelf.Add(hit.Actor.Get());
				DamageInDirection(Direction, ignoreActorsAndSelf);
			}
		}
	}
}


