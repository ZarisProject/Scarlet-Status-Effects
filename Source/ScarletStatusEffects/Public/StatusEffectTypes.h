// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "StatusEffectTypes.generated.h"

// Contains runtime data about an applied status effect
USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

    // Time before status effect will be removed (in seconds)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeRemaining;

    // Pointer to the corresponding UStatusEffect object
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStatusEffect* StatusEffectObject;
};


USTRUCT(BlueprintType)
struct FStatusEffectInfo : public FTableRowBase
{
	GENERATED_BODY()

	// Status effect object class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UStatusEffect> StatusEffectObjectClass;

	// ...
};