// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"

UCLASS()
class ASTARSEARCHPLUGIN_API AGridSpawner : public AActor
{
	GENERATED_BODY()

public:
	AGridSpawner();

	TArray<TArray<AGridCell*>> GetGrid() { return Grid; }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute SetWidth function", Keywords = "GridSpawnerSetWidth"), Category = "AGridSpawner")
	void SetWidth(int W){ Width = W;}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;


public:
	//Editor Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AGridSpawner)
		int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AGridSpawner)
		 int Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AGridSpawner)
		float Margin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AGridSpawner)
		UStaticMesh* Mesh;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Spawn function", Keywords = "GridSpawnerSpawn"), Category = "AGridSpawner")
	void Spawn();

	//Non-Editor Variables
	TArray<TArray<AGridCell*>> Grid;
	USceneComponent* Root;
	
};
