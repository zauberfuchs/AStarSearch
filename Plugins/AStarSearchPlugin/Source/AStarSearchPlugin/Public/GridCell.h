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
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	// Variables
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		bool IsWall;

	USceneComponent* Root;
};
