#pragma once

class SceneObject;

class BaseComponent
{
public:
	virtual void Tick(float deltaTime);
	SceneObject* GetHost();

protected:
	BaseComponent(SceneObject* inHost);

	SceneObject* host;

};