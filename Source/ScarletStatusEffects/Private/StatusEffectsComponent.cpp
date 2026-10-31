// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectsComponent.h"

#include "StatusEffectTypes.h"
#include "StatusEffect.h"
#include "StatusEffectStatic.h"
#include "StatusEffectDynamic.h"
#include "StaticStatusEffectManager.h"

#include "Templates/SubclassOf.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStatusEffectsComponent::UStatusEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = true;
}

// Attempts to find an existing static status effect controller in the world
// If not found, creates a new one
void UStatusEffectsComponent::ObtainStaticController()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticStatusEffectManager::StaticClass(), FoundActors);

	if (FoundActors.IsValidIndex(0))
		StaticManager = Cast<AStaticStatusEffectManager>(FoundActors[0]);

	else
		StaticManager = GetWorld()->SpawnActor<AStaticStatusEffectManager>();
}

// Called when the game starts
void UStatusEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	ObtainStaticController();
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
		RemoveStatusEffectInternal(s, false);
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

	if (!IsActive()) return;

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
	// Extend duration if status effect is already applied
	if (IsStatusEffectApplied(StatusEffect))
	{
		StatusEffects[StatusEffect].TimeRemaining += Duration;
		StatusEffects[StatusEffect].StatusEffectObject->OnReapplied(this, GetOwner());
	}

	// Create a new entry if status effect is not applied yet
	else 
	{
		FStatusEffectData NewData;
		NewData.TimeRemaining = Duration;
		NewData.StatusEffectObject = ObtainStatusEffectObject(StatusEffect);

		StatusEffects.Add(StatusEffect, NewData);

		NewData.StatusEffectObject->OnApplied(this, GetOwner());

		OnStatusEffectApplied.Broadcast(StatusEffect);
	}
}

// Forcefully removes a status effect
// Does nothing if status effect is not applied
void UStatusEffectsComponent::RemoveStatusEffect(const FName& StatusEffect)
{
	RemoveStatusEffectInternal(StatusEffect, true);
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
		return NewObject<UStatusEffectDynamic>(this, ObjectClass->Get());
	}

	// Static case
	else if (ObjectClass->Get()->IsChildOf(UStatusEffectStatic::StaticClass()))
	{
		return StaticManager->RequestStatusEffectObject(StatusEffect, this);
	}

	return nullptr;
}

// Removes status effect from the map
void UStatusEffectsComponent::RemoveStatusEffectInternal(const FName& StatusEffect, bool Forced)
{
	FStatusEffectData* entry = StatusEffects.Find(StatusEffect);
	if (!entry)
		return;
	entry->StatusEffectObject->OnRemoved(this, GetOwner(), Forced);
	StatusEffects.Remove(StatusEffect);

	OnStatusEffectRemoved.Broadcast(StatusEffect, Forced);
}