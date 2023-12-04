// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum EPlayerLobbyTeam
{
	Undecided	= 0,
	Red			= 1,
	Blue		= 2,
	Green		= 3,
	Yellow		= 4,
};

UENUM()
enum EPlayerLobbyRole
{
	User,
	Admin,
	Bot
};