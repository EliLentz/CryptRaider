// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* overlappingActor = GetAcceptableActor();

	if (Mover != nullptr) {
		if (overlappingActor != nullptr && !overlappingActor->ActorHasTag(GrabbedTag)) {

			UPrimitiveComponent* component = Cast<UPrimitiveComponent>(overlappingActor->GetRootComponent());
			if (component != nullptr) {
				component->SetSimulatePhysics(false);
			}

			overlappingActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			Mover->SetShouldMove(true);
		}
		else {
			Mover->SetShouldMove(false);
		}
	}
}

void UTriggerComponent::SetMover(UMover* mover) {
	Mover = mover;
}

AActor* UTriggerComponent::GetAcceptableActor() const {

	AActor* result = nullptr;

	TArray<AActor*> overlappingActors;

	GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		if (actor->ActorHasTag(KeyTag)) {
			result = actor;
		}
	}

	return result;
}