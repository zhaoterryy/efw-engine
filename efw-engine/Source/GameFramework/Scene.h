#pragma once

#include "System/ResourceManager.h"
#include <vector>
#include <string>

namespace sf 
{
	class SoundBuffer;
	class Texture;
}
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

	ResourceManager<sf::SoundBuffer> soundResources;
	ResourceManager<sf::Texture> textureResources;

protected:
	std::vector<Object*> sceneElements;

private:
	std::string name;
};