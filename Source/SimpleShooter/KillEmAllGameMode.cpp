// Fill out your copyright notice in the Description page of Project Settings.

#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn *PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController *PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController)
    {
        // Player lose
        EndGame(false);
    }
    else
    {
        for (AShooterAIController *Controller : TActorRange<AShooterAIController>(GetWorld()))
        {
            // Detect all enemy dead
            if (!Controller->IsDead())
            {
                return;
            }
        }

        // Player win
        EndGame(true);
    }
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController *Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), Controller->IsPlayerController() == bIsPlayerWinner);
    }
}
