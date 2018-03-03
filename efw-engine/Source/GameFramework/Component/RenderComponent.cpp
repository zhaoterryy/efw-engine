#include "RenderComponent.h"
#include "GameFramework/SceneObject.h"

RenderComponent::RenderComponent(SceneObject* inHost) :
	BaseComponent(inHost)
{
}

void RenderComponent::Tick(float deltaTime)
{
	//draw();
}

void RenderComponent::SetTexture(sf::Texture ttr)
{
	sprite.setTexture(ttr);
}

sf::Sprite RenderComponent::GetSprite()
{
	return sprite;
}

void RenderComponent::draw(sf::RenderWindow wd)
{
	if (sprite.getTexture() != nullptr) {
		wd.draw(sprite);
	}
}

