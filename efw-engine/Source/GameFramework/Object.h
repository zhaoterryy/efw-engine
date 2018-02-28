#pragma once

#include "efw-engine/EngineTypes.h"
#include "GameFramework/Component/BaseComponent.h"

#include <vector>
#include <unordered_set>

class Object
{

public:
	Object();

	virtual void Tick(float DeltaTime);


public:
	template <class T>
	T* GetComponent() {
		//for (const auto& comp : Components) {
		for (const BaseComponent* comp : Components) {
			if (typeid(comp) == typeid(T)) {
				return (T*)comp;
			}
		}
		return nullptr;
	}
	
	template <class BaseComponent>
	void AddComponent(BaseComponent) {
		//Check if object already has a component of type, then
		//Components.insert(new BaseComponent(this));
	}

	Object* GetParent();
	void SetParent(Object* obj);
	void AddChild(Object* obj);

protected:
	std::unordered_set <class BaseComponent*> Components;

	Object* Parent;
	std::vector<Object*> Children;

private:
	

};