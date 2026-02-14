// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeSpawner.h" // Note the new include file
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Renamed constructor
ACubeSpawner::ACubeSpawner()
{
	PrimaryActorTick.bCanEverTick = false; 

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Default values for a 5x5 grid
	CubeMesh = nullptr;
	CubeMaterial = nullptr;
	TotalCubesToSpawn = 5000;
	SpawnInterval = 0.5f;
	GridCountX = 5;
	GridCountY = 5;
	GridSpacing = 110.0f;
	CubesSpawned = 0;
}

// Renamed class functions
void ACubeSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 999.f, FColor::White, TEXT("Unreal Vite 26 - PhysX 3.4"), true, FVector2D(2.0f, 2.0f));
	}
	CachedWorld = GetWorld();
	CachedWorld->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACubeSpawner::SpawnGridSnapshot, SpawnInterval, true, 0.0f);
}

void ACubeSpawner::SpawnGridSnapshot()
{
	if (!CubeMesh || GridCountX <= 0 || GridCountY <= 0) return;
	
	const FVector ForwardDir = GetActorForwardVector();
	const FVector RightDir = GetActorRightVector();
	const FVector ActorLoc = GetActorLocation();

	const float StartOffsetX = -((GridCountX - 1) * GridSpacing) / 2.0f;
	const float StartOffsetY = -((GridCountY - 1) * GridSpacing) / 2.0f;

	for (int32 x = 0; x < GridCountX; ++x)
	{
		for (int32 y = 0; y < GridCountY; ++y)
		{
			if (UNLIKELY(CubesSpawned >= TotalCubesToSpawn && TotalCubesToSpawn > 0))
			{
				CachedWorld->GetTimerManager().ClearTimer(SpawnTimerHandle);
				return; 
			}
			const float CurrentOffsetX = StartOffsetX + (x * GridSpacing);
			const float CurrentOffsetY = StartOffsetY + (y * GridSpacing);

			const FVector SpawnLocation = ActorLoc + (ForwardDir * CurrentOffsetX) + (RightDir * CurrentOffsetY);
			
			UStaticMeshComponent* NewCube = NewObject<UStaticMeshComponent>(this);
			NewCube->RegisterComponent();
			NewCube->SetStaticMesh(CubeMesh);
			NewCube->SetWorldLocationAndRotation(SpawnLocation, FRotator::ZeroRotator);
			NewCube->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
			NewCube->SetMaterial(0, CubeMaterial);
			NewCube->SetSimulatePhysics(true);
			CubesSpawned++;
		}
	}

	const FString DebugMessage = FString::Printf(TEXT("Spawned Cubes: %d"), CubesSpawned);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, DebugMessage, true, FVector2D(2.0f, 2.0f));
}