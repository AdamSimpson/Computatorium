#pragma once

// Interface for Actors which can accept a fetchable
class AcceptsFetchable {
public:
	virtual bool CanBindFetchable(class AFetchable *Fetchable) = 0;
	virtual USceneComponent* GetBindingComponent() = 0;
	virtual void BindFetchable(class AFetchable *Fetchable) = 0;
	virtual void PreUnbindFetchable(class AFetchable *Fetchable) {};
	virtual void PostUnbindFetchable(class AFetchable *Fetchable) {};
	virtual void PreBindFetchable(class AFetchable *Fetchable) {};
	virtual void PostBindFetchable(class AFetchable *Fetchable) {};
};