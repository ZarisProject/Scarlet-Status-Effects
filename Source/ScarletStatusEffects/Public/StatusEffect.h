// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "StatusEffectsComponent.h"

#include "StatusEffect.generated.h"

/**
 * Base class for all Status Effect objects
 */
UCLASS()
class SCARLETSTATUSEFFECTS_API UStatusEffect : public UObject
{
	GENERATED_BODY()
	
public:

	// Called when the status effect is applied the first time
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ScarletStatusEffects|StatusEffect")
	void OnApplied(UStatusEffectsComponent* Component, AActor* Actor);
	virtual void OnApplied_Implementation(UStatusEffectsComponent* Component, AActor* Actor) {}

	// Called when the same kind of status effect is applied again (when current one is still active)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ScarletStatusEffects|StatusEffect")
	void OnReapplied(UStatusEffectsComponent* Component, AActor* Actor);
	virtual void OnReapplied_Implementation(UStatusEffectsComponent* Component, AActor* Actor) {}

	// Called every tick while the status effect is still active
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ScarletStatusEffects|StatusEffect")
	void OnTick(UStatusEffectsComponent* Component, AActor* Actor, float DeltaTime);
	virtual void OnTick_Implementation(UStatusEffectsComponent* Component, AActor* Actor, float DeltaTime) {}

	// Called when status effect application time (`TimeRemaining`) runs out 
	// or the status effect is forcefully removed (any way other than the time running out)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ScarletStatusEffects|StatusEffect")
	void OnRemoved(UStatusEffectsComponent* Component, AActor* Actor, bool Forced);
	virtual void OnRemoved_Implementation(UStatusEffectsComponent* Component, AActor* Actor, bool Forced) {}
};
