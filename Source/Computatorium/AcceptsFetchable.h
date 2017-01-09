#pragma once

// Interface like class for Actors which can accept a fetchable
// This won't play nice with blueprints and probably should
// follow UInterface https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Reference/Interfaces/index.html
class AcceptsFetchable {
public:
	// Interface functions which must be implimented
	virtual bool CanBindFetchable(class AFetchable *Fetchable) = 0;
	virtual USceneComponent* GetBindingComponent() = 0;
	virtual void BindFetchable(class AFetchable *Fetchable) = 0;

	// Optional pre/post (un)fetch calls
	virtual void PreUnbindFetchable(class AFetchable *Fetchable) {};
	virtual void PostUnbindFetchable(class AFetchable *Fetchable) {};
	virtual void PreBindFetchable(class AFetchable *Fetchable) {};
	virtual void PostBindFetchable(class AFetchable *Fetchable) {};
};