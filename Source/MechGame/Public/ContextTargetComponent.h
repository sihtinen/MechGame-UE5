// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ContextTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UContextTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EContextLayers))
	int32 ContextLayerFlags = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bReturnCalculatedVelocity = true;

public:

	UContextTargetComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	FVector GetCalculatedVelocity()
	{
		if (bReturnCalculatedVelocity)
			return CalculatedVelocity;
		else
			return GetOwner()->GetVelocity();
	}

private:

	FVector LocationPrevTick;

	FVector CalculatedVelocity = FVector::ZeroVector;
};
