// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeSpawner.generated.h" // Note the new filename here

UCLASS()
class PHYSXTEST_API ACubeSpawner : public AActor // Renamed class
{
	GENERATED_BODY()
	
public:	
	ACubeSpawner(); // Renamed constructor

protected:
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, Category = "Cube Spawner") // Category updated for clarity
	UStaticMesh* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Cube Spawner")
	UMaterialInterface* CubeMaterial;

	UPROPERTY(EditAnywhere, Category = "Cube Spawner", meta = (ClampMin = "0"))
	int32 TotalCubesToSpawn;

	/** The time delay between each GRID snapshot being spawned. */
	UPROPERTY(EditAnywhere, Category = "Cube Spawner", meta = (ClampMin = "0.01"))
	float SpawnInterval;
	
	/** Number of cubes along the actor's Forward axis. */
	UPROPERTY(EditAnywhere, Category = "Grid Settings", meta = (ClampMin = "1"))
	int32 GridCountX;

	/** Number of cubes along the actor's Right axis. */
	UPROPERTY(EditAnywhere, Category = "Grid Settings", meta = (ClampMin = "1"))
	int32 GridCountY;

	/** The distance between centers of the cubes in the grid. */
	UPROPERTY(EditAnywhere, Category = "Grid Settings", meta = (ClampMin = "1.0"))
	float GridSpacing;

private:
	
	FTimerHandle SpawnTimerHandle;

	/** Function that spawns a whole grid of cubes. */
	void SpawnGridSnapshot();

	int32 CubesSpawned;
	
	UWorld* CachedWorld;
};