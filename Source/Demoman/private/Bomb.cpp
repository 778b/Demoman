// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "DamageInterface.h"
#include "Components/StaticMeshComponent.h"



ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = false;
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

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/PS_Explode.PS_Explode"));
	if (MeshAsset.Succeeded())
	{
		ExplodeParticle = ParticleAsset.Object;
	}
}


void ABomb::BlowUp()
{
	OnBlowUpBomb.Execute();
	TArray<AActor*> IgnoreActors = { this };
	FVector EndParticleLocation;
	if (DamageInDirection(FVector(	1.f, 0.f,	0.f), IgnoreActors, EndParticleLocation)) ShowParticle(EndParticleLocation, FColor::Black);
	if (DamageInDirection(FVector(	-1.f, 0.f,	0.f), IgnoreActors, EndParticleLocation)) ShowParticle(EndParticleLocation, FColor::Blue);
	if (DamageInDirection(FVector(	0.f, 1.f,	0.f), IgnoreActors, EndParticleLocation)) ShowParticle(EndParticleLocation, FColor::Emerald);
	if (DamageInDirection(FVector(	0.f, -1.f,	0.f), IgnoreActors, EndParticleLocation)) ShowParticle(EndParticleLocation, FColor::Red);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorTransform(), true, EPSCPoolMethod::AutoRelease);
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
	if (tempArray.Num() == 0) BombMesh->SetCollisionProfileName("BlockAllDynamic");
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


bool ABomb::DamageInDirection(FVector Direction, TArray<AActor*>& ignoreActorsAndSelf, FVector& End)
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

		bool bIsPenetrated = false;
		FVector targetLocation = hit.Actor->GetActorLocation();
		IDamageInterface* tempActor = Cast<IDamageInterface>(hit.Actor.Get());
		if (tempActor)
		{
			tempActor->DamageActor(bIsPenetrated);
			if (bIsPenetrated)
			{
				ignoreActorsAndSelf.Add(hit.Actor.Get());
				return DamageInDirection(Direction, ignoreActorsAndSelf, End);
			}
		}
		if (hit.Actor.IsValid()) // IsValid(hit.Actor.Get())
		{
			if (FVector::Distance(hit.Location, GetActorLocation()) > 100)
			{
				End = hit.Location - (Direction * 50);
				End.X = SetPositionOffset100(End.X);
				End.Y = SetPositionOffset100(End.Y);
				return true;
			}
			else return false;
		}
		else
		{
			End = targetLocation;
			return true;	// Only blocks locations
		}
	}
	End = endTrace;
	return true;
}

void ABomb::ShowParticle(FVector End, FColor color)
{
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, FTransform(FRotator(0.f), End, FVector(1.f)), true, EPSCPoolMethod::AutoRelease);
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
