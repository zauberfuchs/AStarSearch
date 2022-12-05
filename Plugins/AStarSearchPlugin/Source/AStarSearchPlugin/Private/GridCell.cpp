// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCell.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGridCell::AGridCell()
	: parent(-1, -1), f(-1), g(-1), h(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGridCell::OnConstruction(const FTransform& Transform)
{
	SetIsWall();
};

// Called when the game starts or when spawned
void AGridCell::BeginPlay()
{
	Super::BeginPlay();
}