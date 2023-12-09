// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/StaticMesh.h"
#include "DamageInterface.h"
#include "Components/StaticMeshComponent.h"



ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	

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

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/VFX/PS_Explode.PS_Explode"));
	if (MeshAsset.Succeeded())
	{
		ExplodeParticle = ParticleAsset.Object;
	}
}


void ABomb::BlowUp_Implementation()
{
	if (OnBlowUpBomb.IsBound()) OnBlowUpBomb.Execute();
	TArray<AActor*> IgnoreActors = { this };
	DamageInDirection(FVector(1.f, 0.f, 0.f), IgnoreActors);
	DamageInDirection(FVector(-1.f, 0.f, 0.f), IgnoreActors);
	DamageInDirection(FVector(0.f, 1.f, 0.f), IgnoreActors);
	DamageInDirection(FVector(0.f, -1.f, 0.f), IgnoreActors);

	ShowParticleCenter();
	Destroy();
}


void ABomb::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ABomb::BlowUp, 4.f, false);
}

void ABomb::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TSet<AActor*> tempArray;
	BombMesh->GetOverlappingActors(tempArray);
	if (tempArray.Num() == 0)
	{
		BombMesh->SetCollisionProfileName("BlockAllDynamic");
		BombMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
	return;
}

void ABomb::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	return;
}


ABomb* ABomb::SpawnBomb(UWorld* World, FVector Location, int8 Power)
{
	Location.X = SetPositionOffset100(Location.X);
	Location.Y = SetPositionOffset100(Location.Y);

	ABomb* tempBomb = World->SpawnActor<ABomb>(ABomb::StaticClass(), FTransform(FRotator(0.f), Location, FVector(1.f)));
	check(tempBomb);

	tempBomb->Power = Power;
	return tempBomb;
}


void ABomb::DamageInDirection_Implementation(const FVector Direction, const TArray<AActor*>& ignoreActorsAndSelf)
{
	FHitResult hit;
	FVector endTrace = (Power * 100 * Direction) + GetActorLocation();
	FCollisionQueryParams newParam;
	newParam.AddIgnoredActors(ignoreActorsAndSelf);
	bool isHitted = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), endTrace, ECollisionChannel::ECC_Visibility, newParam);

	//DrawDebugLine(GetWorld(), GetActorLocation(), endTrace, FColor::Emerald, false, 3.5f, 0U, 2.f);
	if (isHitted)
	{
		//DrawDebugPoint(GetWorld(), hit.Location, 25.f, FColor::Red, false, 3.5f);
		auto tempActor = hit.GetActor();
		if (IDamageInterface* tempInterface = Cast<IDamageInterface>(tempActor))
		{
			if (tempInterface->DamageActor())
			{
				return DamageInDirection(Direction, ignoreActorsAndSelf);
			}
		}
		if (IsValid(tempActor))
		{
			if (FVector::Distance(hit.Location, GetActorLocation()) > 100)
			{
				FVector End = hit.Location - (Direction * 50);
				End.X = SetPositionOffset100(End.X);
				End.Y = SetPositionOffset100(End.Y);
				ShowParticle(End);
			}
			return;
		}
		ShowParticle(tempActor->GetActorLocation());
		return;
	}
	ShowParticle(endTrace);
}

void ABomb::ShowParticle_Implementation(FVector End)
{
	End.Z = GetActorLocation().Z;
	FVector DirectionBack = GetActorLocation() - End; // EndTrace;		50% of EndTrace;
	DirectionBack.Normalize();
	FVector temp = End;
	int8 i = 0;
	while (true)
	{
		temp = (DirectionBack * 100 * i++) + End;
		if (FVector::Distance(temp, GetActorLocation()) > 75)
		{
			//DrawDebugSphere(GetWorld(), temp, 25.f, 10, color, false, 20.f);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, FTransform(FRotator(0.f), temp, FVector(1.f)), true, EPSCPoolMethod::AutoRelease);
		}
		else break;
	}
}

void ABomb::ShowParticleCenter_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
}

float ABomb::SetPositionOffset100(float num)
{
	int32 temp = FMath::RoundToInt(num) % 100;
	if (num > 0)
	{
		if (temp > 50) return num + 100 - temp;		// 1260 + 100 - 60 = 1300
		else return num - temp;						// 1230 - 30 = 1200
	}
	else
	{
		if (temp < -50) return num - 100 - temp;	// -1480 - 100 + 80 = -1500
		else return num - temp;						// -1320 + 20 = -1300
	}
}
