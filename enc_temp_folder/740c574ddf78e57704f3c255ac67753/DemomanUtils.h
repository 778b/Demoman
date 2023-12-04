// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum EPlayerLobbyTeam
{
	Undecided	= 0b0000,
	Red			= 0b0001,
	Blue		= 0b0010,
	Green		= 0b0100,
	Yellow		= 0b1000,
};

UENUM()
enum EPlayerLobbyRole
{
	User,
	Admin,
	Bot
};