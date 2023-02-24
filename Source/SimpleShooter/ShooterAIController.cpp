// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (!AIBehavior)
    {
        return;
    }

    // Run behavior tree & init
    RunBehaviorTree(AIBehavior);
    GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter *ControllerCharacter = Cast<AShooterCharacter>(GetPawn());
    if (!ControllerCharacter)
    {
        return true;
    }

    return ControllerCharacter->IsDead();
}
