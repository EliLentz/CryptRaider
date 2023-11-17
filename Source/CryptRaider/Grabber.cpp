// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

const FName GRABBED_TAG = "Grabbed";

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if (physicsHandle == nullptr) {
		return;
	}

	if (physicsHandle->GetGrabbedComponent() != nullptr) {
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab() {
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if (physicsHandle == nullptr) {
		return;
	}

	if (physicsHandle->GetGrabbedComponent() != nullptr) {
		return;
	}

	FHitResult hitResult;
	bool hasHit = GetGrabbableInReach(hitResult);

	if (hasHit) {
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->SetSimulatePhysics(true);
		hitComponent->WakeAllRigidBodies();
		AActor* hitActor = hitResult.GetActor();
		hitActor->Tags.Add(GRABBED_TAG);
		hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		physicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);
	}	
}

void UGrabber::Release() {
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if (physicsHandle && physicsHandle->GetGrabbedComponent()) {
		AActor* grabbedActor = physicsHandle->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove(GRABBED_TAG);
		physicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (result == nullptr) {
		UE_LOG(LogTemp, Display, TEXT("Grabber requires a UPhysicsHadleComponent."));
	}

	return result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& hitResult) const {
	FVector start = GetComponentLocation();
	FVector end = start + (GetForwardVector() * MaxGrabDistance);

	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);
}