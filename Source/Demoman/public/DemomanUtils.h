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

	Max			= 5		// Current max player count @todo must be in game mode settings
};

UENUM()
enum EPlayerLobbyRole
{
	User,
	Admin,
	Bot
};


namespace FDemomanUtils
{
	static FString ConvertNetRoleToString(ENetRole EnumToConvert)
	{
		switch (EnumToConvert)
		{
		case ENetRole::ROLE_Authority:
			return "Authority";
		case ENetRole::ROLE_AutonomousProxy:
			return "AutonomousProxy";
		case ENetRole::ROLE_SimulatedProxy:
			return "SimulatedProxy";
		}
		return "None";
	}

	static FString ConvertNetModeToString(ENetMode EnumToConvert)
	{
		switch (EnumToConvert)
		{
		case ENetMode::NM_Client:
			return "Client";
		case ENetMode::NM_DedicatedServer:
			return "DedicatedServer";
		case ENetMode::NM_ListenServer:
			return "ListenServer";
		case ENetMode::NM_Standalone:
			return "Standalone";
		}
		return "None";
	}
	
}