#pragma once

class SceneObject;

class BaseComponent
{
public:
	virtual void Tick(float deltaTime);
	SceneObject* GetOwner();

protected:
	BaseComponent(SceneObject* inHost);

	SceneObject* host;

};