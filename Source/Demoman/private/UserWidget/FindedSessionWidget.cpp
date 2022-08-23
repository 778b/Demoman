// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/FindedSessionWidget.h"

#include "Interfaces/OnlineSessionInterface.h"


void UFindedSessionWidget::TryJoinSession()
{
	const IOnlineSessionPtr SessionPtr = Online::GetSessionInterface(GetOwningPlayer()->GetWorld());
	if (!SessionPtr.IsValid())
	{
		return;
	}
	//SessionPtr->AddOnJoinSessionCompleteDelegate_Handle()

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	SessionPtr->JoinSession(*localPlayer->GetPreferredUniqueNetId(), FName(), SessionResult);

}
