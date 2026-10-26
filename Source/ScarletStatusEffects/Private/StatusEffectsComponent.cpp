// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectsComponent.h"

#include "StatusEffectTypes.h"
#include "StatusEffect.h"
#include "StatusEffectStatic.h"
#include "StatusEffectDynamic.h"

#include "Templates/SubclassOf.h"

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

// Updates remaining time for all applied status effects
void UStatusEffectsComponent::UpdateTimers(float DeltaTime)
{
	TArray<FName> StatusEffectsToRemove;
	
	// Updating timers
	for (auto& entry : StatusEffects)
	{
		entry.Value.TimeRemaining -= DeltaTime;
		if (entry.Value.TimeRemaining <= 0.f)
			StatusEffectsToRemove.Add(entry.Key);
	}

	// Terminating overtime status effects
	for (auto& s : StatusEffectsToRemove)
	{
		StatusEffects[s].StatusEffectObject->OnRemoved(this, GetOwner(), false);
		StatusEffects.Remove(s);
	}
}

// Calls OnTick on every applied status effect
void UStatusEffectsComponent::UpdateStatusEffects(float DeltaTime)
{
	for (auto& entry : StatusEffects)
	{
		entry.Value.StatusEffectObject->OnTick(this, GetOwner(), DeltaTime);
	}
}


// Called every frame
void UStatusEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calling update methods
	UpdateTimers(DeltaTime);
	UpdateStatusEffects(DeltaTime);
}

// Returns current remaining time (in seconds) for the specified status effect, 0.0 if status effect is not applied
float UStatusEffectsComponent::GetTimeRemaining(const FName& StatusEffect)
{
	FStatusEffectData* entry = StatusEffects.Find(StatusEffect);
	if (!entry)
		return 0.0;
	return entry->TimeRemaining;
}

// Modifies current remaining time (in seconds) for the specified status effect
// Does nothing if status effect is not applied
void UStatusEffectsComponent::SetTimeRemaining(const FName& StatusEffect, float NewTime)
{
	FStatusEffectData* entry = StatusEffects.Find(StatusEffect);
	if (!entry)
		return;
	entry->TimeRemaining = NewTime;
}


// Applies a new status effect (or extends the remaining time of an exising one)
void UStatusEffectsComponent::ApplyStatusEffect(const FName& StatusEffect, float Duration)
{
	
}

// Forcefully removes a status effect
// Does nothing if status effect is not applied
void UStatusEffectsComponent::RemoveStatusEffect(const FName& StatusEffect)
{
	FStatusEffectData* entry = StatusEffects.Find(StatusEffect);
	if (!entry)
		return;
	entry->StatusEffectObject->OnRemoved(this, GetOwner(), true);
	StatusEffects.Remove(StatusEffect);
}


// UTILITY

// Whether StatusEffectInfoLibrary contains info about the specified status effect
bool UStatusEffectsComponent::IsStatusEffectValid(const FName& StatusEffect)
{
	for (auto Table : StatusEffectInfoLibrary)
	{
		FStatusEffectInfo* data = Table->FindRow<FStatusEffectInfo>(StatusEffect, 
			TEXT("SCARLET STATUS EFFECTS: Find status effect"));

		if (data)
			return true;
	}
	return false;
}

// Finds the object class of the specified status effect
TSubclassOf<UStatusEffect>* UStatusEffectsComponent::GetStatusEffectObjectClass(const FName& StatusEffect)
{
	for (auto Table : StatusEffectInfoLibrary)
	{
		FStatusEffectInfo* data = Table->FindRow<FStatusEffectInfo>(StatusEffect, 
			TEXT("SCARLET STATUS EFFECTS: Find status effect"));

		if (data)
			return &(data->StatusEffectObjectClass);
	}
	return nullptr;
}

// Creates (dynamic) or requests (static) a status effect object
UStatusEffect* UStatusEffectsComponent::ObtainStatusEffectObject(const FName& StatusEffect)
{
	TSubclassOf<UStatusEffect>* ObjectClass = GetStatusEffectObjectClass(StatusEffect);

	// Return nullptr when no class was found
	if (!ObjectClass)
		return nullptr;

	// Dynamic case
	if (ObjectClass->Get()->IsChildOf(UStatusEffectDynamic::StaticClass()))
	{
		// TO DO: Spawn status effect object
	}

	// Static case
	else if (ObjectClass->Get()->IsChildOf(UStatusEffectStatic::StaticClass()))
	{
		// TO DO: Request status effect object from status manager
	}

	return nullptr;
}