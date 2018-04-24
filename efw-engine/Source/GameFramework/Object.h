#pragma once

#include <string>

class Scene;
class Object
{
public:
	Object() = default;

	virtual void Tick(float deltaTime);

	void SetName(std::string inName);
	inline std::string GetName() { return name; }

	void SetScene(Scene* scene);
	Scene* GetScene() const { return scene; }
private:
	std::string name;
	Scene* scene;
};