#pragma once

class Object;

class BaseComponent
{

public:
	BaseComponent();

	BaseComponent(Object* InHost);

	virtual void Tick(float DeltaTime);

	Object* GetHost();

protected:
	Object* Host;

private:

};