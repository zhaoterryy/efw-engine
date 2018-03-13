#pragma once

#include <vector>
#include <string>

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

	std::vector<Object*>::const_iterator GetElementsBegin() { return sceneElements.begin(); }
	std::vector<Object*>::const_iterator GetElementsEnd() { return sceneElements.end(); }

protected:
	std::vector<Object*> sceneElements;

private:
	std::string name;
};