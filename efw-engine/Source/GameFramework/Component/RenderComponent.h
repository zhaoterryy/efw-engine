#pragma once

#include "SFML/Graphics.hpp"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Object.h"
#include "BaseComponent.h"

class RenderComponent : public BaseComponent
{

public:
	RenderComponent(SceneObject* inHost);

	virtual void Tick(float deltaTime) override;

	void SetTexture(sf::Texture ttr);
	sf::Sprite GetSprite();

protected:
	void draw(sf::RenderWindow wd);

private:
	sf::Sprite sprite;

};