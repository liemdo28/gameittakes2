// Copyright Shards of Dawn. All Rights Reserved.

#include "Actors/PuzzleActors/SODPuzzleDoor.h"
#include "Components/StaticMeshComponent.h"

ASODPuzzleDoor::ASODPuzzleDoor()
	: OpenOffset(FVector(0, 0, 300.0f))
	, OpenSpeed(2.0f)
	, bIsOpen(false)
{
	bReplicates = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);
	DoorMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ASODPuzzleDoor::BeginPlay()
{
	Super::BeginPlay();

	// Bind to all linked puzzles
	for (ASODPuzzleActor* Puzzle : LinkedPuzzles)
	{
		if (Puzzle)
		{
			Puzzle->OnPuzzleStateChanged.AddDynamic(this, &ASODPuzzleDoor::OnLinkedPuzzleStateChanged);
		}
	}
}

void ASODPuzzleDoor::OnLinkedPuzzleStateChanged(ESODPuzzleState NewState)
{
	if (AreAllPuzzlesSolved() && !bIsOpen)
	{
		OpenDoor();
	}
	else if (!AreAllPuzzlesSolved() && bIsOpen)
	{
		CloseDoor();
	}
}

bool ASODPuzzleDoor::AreAllPuzzlesSolved() const
{
	for (const ASODPuzzleActor* Puzzle : LinkedPuzzles)
	{
		if (!Puzzle || Puzzle->GetPuzzleState() != ESODPuzzleState::Solved)
		{
			return false;
		}
	}
	return LinkedPuzzles.Num() > 0;
}

void ASODPuzzleDoor::OpenDoor_Implementation()
{
	bIsOpen = true;
	// In production: use timeline or interp for smooth animation
	DoorMesh->SetRelativeLocation(DoorMesh->GetRelativeLocation() + OpenOffset);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASODPuzzleDoor::CloseDoor_Implementation()
{
	bIsOpen = false;
	DoorMesh->SetRelativeLocation(DoorMesh->GetRelativeLocation() - OpenOffset);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
