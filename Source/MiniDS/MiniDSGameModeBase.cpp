// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniDSGameModeBase.h"
#include "PaperCharacter.h"

AMiniDSGameModeBase::AMiniDSGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APaperCharacter> BP_Char(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/BP_Weber.BP_Weber_C'"));
	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}
}
