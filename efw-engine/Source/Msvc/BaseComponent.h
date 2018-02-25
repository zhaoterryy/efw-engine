#pragma once

#include "..\GameFramework\Object.h"

class BaseComponent
{

public:
	BaseComponent();

	BaseComponent(class Object* host);

	virtual void Tick(float DeltaTime);

	Object* GetHost();

protected:
	class Object* Host;

private:

};