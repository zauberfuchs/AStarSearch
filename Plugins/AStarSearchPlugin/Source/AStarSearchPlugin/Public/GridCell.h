// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.generated.h"

UCLASS()
class ASTARSEARCHPLUGIN_API AGridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridCell();
	
	void SetMeshComponent(UStaticMesh* M) const { Mesh->SetStaticMesh(M); }
	UStaticMeshComponent* GetMeshComponent() const { return Mesh; }

	bool GetIsWall() { return IsWall; }
	
	// Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	FVector2D parent;
	// f = g + h
	double f, g, h;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	// Variables
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	
	void SetIsWall() { IsWall ? Mesh->SetRelativeRotation(FRotator(0, 90, 0)) : Mesh->SetRelativeRotation(FRotator(0, 0, 0)); }
	
	UPROPERTY(EditAnywhere)
		bool IsWall = false;

	USceneComponent* Root;
};
