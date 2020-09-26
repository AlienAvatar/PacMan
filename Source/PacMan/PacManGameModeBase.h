// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/Enemy.h"
#include "EngineUtils.h"
#include "PacManGameModeBase.generated.h"

/**
 * 
 */

enum class EGameState : short {
	EMenu,
	EPlaying,
	EPause,
	EWin,
	EGameOver
};

UCLASS()
class PACMAN_API APacManGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	EGameState GetCurrentState() const;

	void SetCurrentState(EGameState value);

	void SetVulnerableEnemy();
private:
	EGameState currentState;
	TArray<class AEnemy*> Enemys;
};

FORCEINLINE EGameState APacManGameModeBase::GetCurrentState() const {
	return currentState;
}
