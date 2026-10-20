// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectsComponent.h"

// Sets default values for this component's properties
UStatusEffectsComponent::UStatusEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Returns current remaining time (in seconds) for the specified status effect, 0.0 if status effect is not applied
float UStatusEffectsComponent::GetTimeRemaining(const FName& StatusEffect)
{
	return 0.0;
}

// Modifies current remaining time (in seconds) for the specified status effect
// Does nothing if status effect is not applied
void UStatusEffectsComponent::SetTimeRemaining(const FName& StatusEffect, float NewTime)
{

}


// Applies a new status effect (or extends the remaining time of an exising one)
void UStatusEffectsComponent::ApplyStatusEffect(const FName& StatusEffect, float Duration)
{

}

// Forcefully removes a status effect
// Does nothing if status effect is not applied
void UStatusEffectsComponent::RemoveStatusEffect(const FName& StatusEffect)
{

}