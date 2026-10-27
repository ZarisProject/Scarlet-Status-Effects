// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffectStatic.h"
#include "StaticStatusEffectManager.generated.h"

UCLASS()
class SCARLETSTATUSEFFECTS_API AStaticStatusEffectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticStatusEffectManager();

protected:

	// Buffer of all static status effect objects that have been created
	UPROPERTY()
	TMap<FName, UStatusEffectStatic*> StaticStatusEffects;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Creates a status effect object based on class, provided from component's info library
	UStatusEffectStatic* CreateStatusEffectObject(const FName& StatusEffect, UStatusEffectsComponent* StatusEffectComponent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Provides a pointer to a UStatusEffectStatic that matches the specified status effect
	UFUNCTION(BlueprintCallable, Category="ScarletStatusEffects|Static")
	UStatusEffectStatic* RequestStatusEffectObject(const FName& StatusEffect, UStatusEffectsComponent* StatusEffectComponent);
};
