// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacManGameModeBase.h"

void APacManGameModeBase::SetCurrentState(EGameState value)
{
	currentState = value;

	switch (value)
	{
	case EGameState::EPlaying:
		for (auto Iter(Enemys.CreateConstIterator()); Iter; Iter++) {
			(*Iter)->SetMove(true);
		}
		break;
	case EGameState::EPause:
		for (auto Iter(Enemys.CreateConstIterator()); Iter; Iter++) {
			(*Iter)->SetMove(false);
		}
		break;
	case EGameState::EWin:
		for (auto Iter(Enemys.CreateConstIterator()); Iter; Iter++) {
			(*Iter)->Destroy();
		}
		break;
	case EGameState::EGameOver:
		for (auto Iter(Enemys.CreateConstIterator()); Iter; Iter++) {
			(*Iter)->Destroy();
		}
		break;
	default:
		break;
	}
}

void APacManGameModeBase::SetVulnerableEnemy()
{
	for (auto Iter(Enemys.CreateConstIterator()); Iter;Iter++) {
		(*Iter)->SetVulnerable();
	}
}

void APacManGameModeBase::BeginPlay() {
	SetCurrentState(EGameState::EMenu);

	for (TActorIterator<AEnemy> enemyItr(GetWorld()); enemyItr; ++enemyItr) {
		AEnemy* enemy = Cast<AEnemy>(*enemyItr);
		if (enemy) {
			Enemys.Add(enemy);
		}
	}
}
