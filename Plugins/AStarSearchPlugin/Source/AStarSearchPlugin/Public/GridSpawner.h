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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;


private:
	//Editor Variables
	UPROPERTY(EditAnywhere)
		unsigned int Width;
	UPROPERTY(EditAnywhere)
		unsigned int Height;
	UPROPERTY(EditAnywhere)
		float Margin;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;
	
	void Spawn();

	//Non-Editor Variables
	TArray<TArray<AGridCell*>> Grid;
	USceneComponent* Root;
};
