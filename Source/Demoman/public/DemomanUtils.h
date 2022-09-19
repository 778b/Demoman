// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum EPlayerLobbyTeam
{
	Undecided,
	Red,
	Blue,
	Green,
	Yellow
};

UENUM()
enum EPlayerLobbyRole
{
	User,
	Admin,
	Bot
};