#pragma once

#include "Object.h"
#include "Component/BaseComponent.h"
#include "Component/TransformComponent.h"

#include <type_traits>
#include <vector>
#include <unordered_set>
#include <memory>

class SceneObject : public Object
{
public:
	SceneObject() = default;

	virtual void Tick(float deltaTime) override;

	template <class T>
	T* GetComponent();

	template <class T>
	T& AddComponent();

	template <class T, class ... Vargs>
	T& AddComponent(Vargs ... args);

	inline SceneObject* GetParent() { return parent; }

	void SetParent(SceneObject* inObj);
	void AddChild(SceneObject* inObj);

protected:
	std::vector<std::unique_ptr<BaseComponent>> components;

	SceneObject* parent;
	std::vector<SceneObject*> children;
};

template <class T>
T* SceneObject::GetComponent()
{
	static_assert(std::is_base_of<BaseComponent, T>::value, "GetComponent<T>(): T must be derived from BaseComponent");
	for (auto& comp : components)
		if (typeid(*comp) == typeid(T))
			return static_cast<T*>(&*comp);

	return nullptr;
}

template <class T>
T& SceneObject::AddComponent()
{
	static_assert(std::is_base_of<BaseComponent, T>::value, "AddComponent<T>(): T must be derived from BaseComponent");
	components.push_back(std::make_unique<T>(this));
	return static_cast<T&>(*components.back());
}

template <class T, class ... Vargs>
T& SceneObject::AddComponent(Vargs ... args)
{
	static_assert(std::is_base_of<BaseComponent, T>::value, "AddComponent<T>(): T must be derived from BaseComponent");
	components.push_back(std::make_unique<T>(this, args ...));
	return static_cast<T&>(*components.back());
}