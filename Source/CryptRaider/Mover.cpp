// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();

}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldMove) {
		Move(DeltaTime, MoveOffset);
	}
	else if (ShouldReturn) {
		Move(DeltaTime, MoveOffset * 0);
	}
}

void UMover::SetShouldMove(bool value) {
	ShouldMove = value;
}

void UMover::Move(float DeltaTime, FVector moveOffset) {
	FVector currentLocation = GetOwner()->GetActorLocation();
	FVector targetLocation = OriginalLocation + moveOffset;
	float speed = FVector::Distance(currentLocation, targetLocation) / MoveTime;

	FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, DeltaTime, speed);
	GetOwner()->SetActorLocation(newLocation);
}
