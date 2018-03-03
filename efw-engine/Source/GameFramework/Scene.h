#pragma once

#include <vector>

class Object;

class Scene
{

public:
	Scene();

	virtual void Tick(float deltaTime);
	
	void AddObject(Object* obj);

	// @test
	void TestPrintObjectTransforms();

	void SetSceneName(std::string inName);
	std::string GetSceneName() { return name; }

protected:
	std::vector<Object*> sceneElements;

private:
	std::string name;
};