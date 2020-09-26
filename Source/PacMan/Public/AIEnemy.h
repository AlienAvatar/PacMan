// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Public/Enemy.h"
#include "../PacManGameModeBase.h"
#include "AIEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AAIEnemy : public AAIController
{
	GENERATED_BODY()

public:
	void OnPossess(class APawn* InPawn) override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void SearchNewPoint();

	void StopMove();
	void ReArm();
	void GoHome();
	
private:
	class AEnemy* Bot;
	FTimerHandle DeadTime;
	FVector HomeLocation;
	APacManGameModeBase* GameMode;
};
