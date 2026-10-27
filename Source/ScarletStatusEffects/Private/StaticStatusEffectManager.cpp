// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticStatusEffectManager.h"
#include "StatusEffectStatic.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
AStaticStatusEffectManager::AStaticStatusEffectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStaticStatusEffectManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticStatusEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Creates a status effect object based on class, provided from component's info library
UStatusEffectStatic* AStaticStatusEffectManager::CreateStatusEffectObject(const FName& StatusEffect, UStatusEffectsComponent* StatusEffectComponent)
{
	TSubclassOf<UStatusEffect>* ObjectClass = StatusEffectComponent->GetStatusEffectObjectClass(StatusEffect);
	if (!ObjectClass->Get()->IsChildOf(UStatusEffectStatic::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SCARLET STATUS EFFECTS: Requested a non-static status effect from status manager!"));
		return nullptr;
	}

	return NewObject<UStatusEffectStatic>(this, ObjectClass->Get());
}

// Provides a pointer to a UStatusEffectStatic that matches the specified status effect
UStatusEffectStatic* AStaticStatusEffectManager::RequestStatusEffectObject(const FName& StatusEffect, UStatusEffectsComponent* StatusEffectComponent)
{
	UStatusEffectStatic** entry = StaticStatusEffects.Find(StatusEffect);
	if (entry)
		return *(entry);

	UStatusEffectStatic* NewObject = CreateStatusEffectObject(StatusEffect, StatusEffectComponent);
	StaticStatusEffects.Add(StatusEffect, NewObject);

	return NewObject;
}

