#include "AsyncTickCallback.h"

void FAsyncPhysicsCallback::OnPreSimulate_Internal()
{
	using namespace Chaos;

	const FAsyncPhysicsInput* Input = GetConsumerInput_Internal();

	if(Input == nullptr)
		return;

	UWorld* World = Input->World.Get();

	if(World == nullptr)
		return;

	const TArray<TWeakObjectPtr<AAsyncTickPawn>>& Pawns = Input->Pawns;

	for(const TWeakObjectPtr<AAsyncTickPawn> Pawn : Pawns)
	{
		if(!Pawn.IsValid())
			continue;
		
		Pawn->NativeAsyncTick(GetDeltaTime_Internal());
	}
}

void FAsyncPhysicsCallback::OnContactModification_Internal(Chaos::FCollisionContactModifier& Modifier)
{
}