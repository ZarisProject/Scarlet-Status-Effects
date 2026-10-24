// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "StatusEffectTypes.h"

#include "StatusEffectsComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectApplied, const FName&, StatusEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectRemoved, const FName&, StatusEffect, bool, Forced);

// Class
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCARLETSTATUSEFFECTS_API UStatusEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectsComponent();

protected:
	
	// Map of all currently applied status effects
	TMap<FName, FStatusEffectData> StatusEffects;

	// Cached pointer to the world's static status effect manager
	class AStaticStatusEffectManager* StaticManager;

public:

	// Set of status effect data tables, providing information about every status effect
	// that can be applied to this component's owner
	// ALL DATATABLES ARE EXPECTED TO BE OF FStatusEffectInfo TYPE!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	TSet<UDataTable*> StatusEffectInfoLibrary;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// UPDATERS

	// Updates remaining time for all applied status effects
	void UpdateTimers(float DeltaTime);

	// Calls OnTick on every applied status effect
	void UpdateStatusEffects(float DeltaTime);


	// UTILITY
	
	// Finds the object class of the specified status effect
	TSubclassOf<UStatusEffect>* GetStatusEffectObjectClass(const FName& StatusEffect);

	// Creates (dynamic) or requests (static) a status effect object
	UStatusEffect* ObtainStatusEffectObject(const FName& StatusEffect);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns (puts into outStatusEffects) names of all currently applied status effects
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="ScarletStatusEffects")
	void GetAppliedStatusEffects(TArray<FName>& OutStatusEffects) {  StatusEffects.GetKeys(OutStatusEffects); }

	// Returns true if the specified status effect is currently applied, false - if otherwise
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="ScarletStatusEffects")
	bool IsStatusEffectApplied(const FName& StatusEffect) { return StatusEffects.Contains(StatusEffect); }

	
	// Returns current remaining time (in seconds) for the specified status effect, 0.0 if status effect is not applied
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="ScarletStatusEffects")
	float GetTimeRemaining(const FName& StatusEffect);

	// Modifies current remaining time (in seconds) for the specified status effect
	// Does nothing if status effect is not applied
	UFUNCTION(BlueprintCallable, Category="ScarletStatusEffects")
	void SetTimeRemaining(const FName& StatusEffect, float NewTime);


	// Applies a new status effect (or extends the remaining time of an exising one)
	UFUNCTION(BlueprintCallable, Category="ScarletStatusEffects")
	void ApplyStatusEffect(const FName& StatusEffect, float Duration);

	// Forcefully removes a status effect
	// Does nothing if status effect is not applied
	UFUNCTION(BlueprintCallable, Category="ScarletStatusEffects")
	void RemoveStatusEffect(const FName& StatusEffect);


	// Whether StatusEffectInfoLibrary contains info about the specified status effect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="ScarletStatusEffects")
	bool IsStatusEffectValid(const FName& StatusEffect);

public:
	// Delegates

	// Fired when a new status effect is applied
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnStatusEffectApplied OnStatusEffectApplied;

	// Fired when any status effect is removed (forced or by time)
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnStatusEffectRemoved OnStatusEffectRemoved;
};
