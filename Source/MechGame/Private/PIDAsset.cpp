// Fill out your copyright notice in the Description page of Project Settings.


#include "PIDAsset.h"

void UPIDAsset::UpdateTick(FPIDState& State, const float& DeltaTime, const float& CurrentValue, const float& TargetValue)
{
    float Error = TargetValue - CurrentValue;

    float P = ProportionalGain * Error;

    State.IntegrationStored = FMath::Clamp(State.IntegrationStored + Error * DeltaTime, -IntegralLimit, IntegralLimit);
    float I = IntegralGain * State.IntegrationStored;

    float _errorChangeRate = (Error - State.PrevError) / DeltaTime;
    State.PrevError = Error;

    float _valueChangeRate = (CurrentValue - State.PrevValue) / DeltaTime;
    State.PrevValue = CurrentValue;

    float DerivativeMeasure = 0;

    if (State.bDerivativeInitialized)
    {
        switch (DerivativeMeasurementType)
        {
        default:
        case FDerivativeMeasurementType::Velocity:
            DerivativeMeasure = -_valueChangeRate;
            break;

        case FDerivativeMeasurementType::ErrorChangeRate:
            DerivativeMeasure = _errorChangeRate;
            break;
        }
    }
    else
    {
        State.bDerivativeInitialized = true;
    }

    float D = DerivativeGain * DerivativeMeasure;

    State.Output = P + I + D;
    State.Output = FMath::Clamp(State.Output, OutputMin, OutputMax);

    if (bEnabled == false)
        State.Output = 0;

    State.P = P;
    State.I = I;
    State.D = D;
}

void UPIDAsset::UpdateAngleTick(FPIDState& State, const float& DeltaTime, const float& CurrentAngle, const float& TargetAngle)
{
    float Error = AngleDifference(TargetAngle, CurrentAngle);

    float P = ProportionalGain * Error;

    State.IntegrationStored = FMath::Clamp(State.IntegrationStored + (Error * DeltaTime), -IntegralLimit, IntegralLimit);
    float I = IntegralGain * State.IntegrationStored;

    float ErrorRateOfChange = AngleDifference(Error, State.PrevError) / DeltaTime;
    State.PrevError = Error;

    float ValueRateOfChange = AngleDifference(CurrentAngle, State.PrevValue) / DeltaTime;
    State.PrevValue = CurrentAngle;

    float DeriveMeasure = 0;

    if (State.bDerivativeInitialized)
    {
        switch (DerivativeMeasurementType)
        {
            default:
            case FDerivativeMeasurementType::Velocity:
                DeriveMeasure = -ValueRateOfChange;
                break;

            case FDerivativeMeasurementType::ErrorChangeRate:
                DeriveMeasure = ErrorRateOfChange;
                break;
        }
    }
    else
    {
        State.bDerivativeInitialized = true;
    }

    float D = DerivativeGain * DeriveMeasure;

    State.Output = P + I + D;
    State.Output = FMath::Clamp(State.Output, OutputMin, OutputMax);

    if (bEnabled == false)
        State.Output = 0;
}

//calculate modular difference, and remap to [-180, 180]
float UPIDAsset::AngleDifference(const float& a, const float& b)
{
    float StepA = FMath::Fmod((a - b + 540), 360);
    float Result = StepA - 180;
    return Result;
}