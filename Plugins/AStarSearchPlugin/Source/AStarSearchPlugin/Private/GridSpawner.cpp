// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSpawner.h"

// Sets default values
AGridSpawner::AGridSpawner()
	: Width(0), Height(0), Margin(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = Root;
}

void AGridSpawner::Spawn()
{
	UWorld* world = GetWorld();

	//Delete Actors and clear Grid
	for(auto row : Grid)
	{
		for(auto cell : row)
		{
			world->DestroyActor(cell);
		}
	}
	Grid.Empty();

	FVector Location = { 0.0f, 0.0f, 0.0f };
	FRotator Rotation = { 0.0f, 0.0f, 0.0f };
	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < Width; i++)
	{
		Location = { 0, Margin * i, 0 };
		Grid.Add(TArray<AGridCell*>());

		for (int j = 0; j < Height; j++)
		{
			Grid[i].Add(world->SpawnActor<AGridCell>(Location, Rotation, SpawnInfo));
			FAttachmentTransformRules Ar(EAttachmentRule::KeepRelative, true);
			Grid[i][j]->AttachToActor(this, Ar);

			if(Mesh != NULL)
			{
				Grid[i][j]->SetMeshComponent(Mesh);
			}
			Location += {Margin, 0, 0};
		}
	}
}

void AGridSpawner::OnConstruction(const FTransform& Transform)
{
	Spawn();
};

// Called when the game starts or when spawned
void AGridSpawner::BeginPlay()
{
	Super::BeginPlay();
}

