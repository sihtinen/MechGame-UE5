// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechDataStructures.h"
#include "MechTargetingComponent.generated.h"

class UContextTargetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMechTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EContextLayers))
	int32 TargetContextLayerFlags = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTargetingOption> ActiveTargetingOptions;

public:

	UMechTargetingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<TWeakObjectPtr<UContextTargetComponent>> ValidContextTargetsArray;
};
