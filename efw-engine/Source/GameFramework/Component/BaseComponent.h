#pragma once

class SceneObject;

class BaseComponent
{

public:
	BaseComponent(SceneObject* inHost);

	virtual void Tick(float deltaTime);

	SceneObject* GetHost();

protected:
	SceneObject* host;

};