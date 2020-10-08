// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AProceduralGeneration::AProceduralGeneration()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
    RootComponent = LocationComponent;
    StartTimeBetweenRoom = 0.25f;
    MoveAmount = 1000;
    ZPos = 150.0f;
}

// Called when the game starts or when spawned
void AProceduralGeneration::BeginPlay()
{
    Super::BeginPlay();

    Direction = FMath::RandRange(0, 5);
    ChooseStartingPoint();
}

// Called every frame
void AProceduralGeneration::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bChooseNewStartingPoint)
    {
        ChooseStartingPoint();
    }
    
    if (!bIsOutOfBounds)
    {
        if (TimeBetweenRoom <= 0)
        {
            Move();
            TimeBetweenRoom = StartTimeBetweenRoom;
        }
        else
        {
            TimeBetweenRoom -= DeltaTime;
        }
    }
}

void AProceduralGeneration::ChooseStartingPoint()
{
    int RandStartingPos = FMath::RandRange(0, StartingLocations.Num() - 1);
    SetActorLocation(StartingLocations[RandStartingPos]->GetActorLocation());
    AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[0], GetActorLocation(), FRotator(0.f, 0.f, 0.f));
    bChooseNewStartingPoint = false;
}

void AProceduralGeneration::Move()
{
    // Goes To the right
    if (Direction == 1 || Direction == 2)
    {
        if (GetActorLocation().Y > MinY)
        {
            FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y - MoveAmount, ZPos);
            SetActorLocation(NewPos);

            // Spawns a random room
            int Rand = FMath::RandRange(0, Rooms.Num()-1);
            AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);

            // Makes sure it can only move right or down
            Direction = FMath::RandRange(1,5);
            if (Direction == 3)
            {
                Direction = 2;
            }
            else if (Direction == 4)
            {
                Direction = 5;
            }
        }
        else
        {
            Direction = 5;
        }
    }

    // Goes to the left
    else if (Direction == 3 || Direction == 4)
    {
        if (GetActorLocation().Y < MaxY)
        {
            FVector NewPos = FVector(GetActorLocation().X,GetActorLocation().Y + MoveAmount,  ZPos);
            SetActorLocation(NewPos);

            // Spawns a random room
            int Rand = FMath::RandRange(0, Rooms.Num()-1);
            AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);
            
            // Makes sure it can only move left or down
            Direction = FMath::RandRange(3,5);

            if (Direction == 1)
            {
                Direction = 3;
            }
            else if (Direction == 2)
            {
                Direction = 5;
            }
        }
        else
        {
            Direction = 5;
        }
    }

    // Goes down
    else if (Direction == 5)
    {
        if (GetActorLocation().X < MaxX)
        {
            TArray<AActor*> RoomHit;
            TArray<AActor*> ActorsToIgnore;
            UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 200.0f, ActorsToCheck, ActorClass, ActorsToIgnore, RoomHit);
            
            FVector NewPos = FVector(GetActorLocation().X + MoveAmount, GetActorLocation().Y , ZPos);
            SetActorLocation(NewPos);

            // Spawns a random that has a top opening
            int Rand = FMath::RandRange(2, 3);
            AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);
            
            // Choose any new direction
            Direction = FMath::RandRange(1, 5);
        }
        else
        {
            bIsOutOfBounds = true;
        }
    }
    AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[0], GetActorLocation(), FRotator::ZeroRotator);
}
