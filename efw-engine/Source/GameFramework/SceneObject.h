#pragma once

#include "Object.h"
#include "Component/BaseComponent.h"
#include "Component/TransformComponent.h"

#include <type_traits>
#include <vector>
#include <unordered_set>

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
	T& AddComponent(Vargs ...);

	void AddComponent(BaseComponent* inComponent);

	inline SceneObject* GetParent() { return parent; }

	void SetParent(SceneObject* inObj);
	void AddChild(SceneObject* inObj);

protected:
	std::vector<BaseComponent*> components;

	SceneObject* parent;
	std::vector<SceneObject*> children;
};

template <class T>
T* SceneObject::GetComponent()
{
	static_assert(std::is_base_of<BaseComponent, T>::value, "GetComponent<T>(): T must be derived from BaseComponent");

	for (const BaseComponent* comp : components)
	{
		if (typeid(*comp) == typeid(T))
		{
			return (T*)comp;
		}
	}
	return nullptr;
}

template <> inline TransformComponent& SceneObject::AddComponent<TransformComponent>(FVector p, float r, FVector s)
{
	return *new TransformComponent(this, p, r, s);
}

template <> inline TransformComponent& SceneObject::AddComponent<TransformComponent>(FVector p, int r, FVector s)
{
	return *new TransformComponent(this, p, (float) r, s);
}

template <> inline TransformComponent& SceneObject::AddComponent<TransformComponent>(FVector p, double r, FVector s)
{
	return *new TransformComponent(this, p, (float) r, s);
}

template <> inline TransformComponent& SceneObject::AddComponent<TransformComponent>(FTransform transform)
{
	return *new TransformComponent(this, transform);
}

template <class T>
T& SceneObject::AddComponent()
{
	static_assert(std::is_base_of<BaseComponent, T>::value, "AddComponent<T>(): T must be derived from BaseComponent");

	return *new T(this);
}

template <class T, class ... Vargs>
T& SceneObject::AddComponent(Vargs ...)
{
	return AddComponent<T>();
}
