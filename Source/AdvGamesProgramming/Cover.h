// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverNode.h"
#include "NavigationNode.h"
#include "Cover.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API ACover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attached Nodes")
	TArray<ANavigationNode*> AttachedNodes;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckForNavNodes();
};
