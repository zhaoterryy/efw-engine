#include "Renderer.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"
#include "GameFramework/Component/RenderComponent.h"

#include <SFML/Graphics.hpp>

Renderer::Renderer(sf::RenderWindow& rw) :
	renderWindow(rw)
{
}

// i really want to optimize this better.......... but nah
void Renderer::Draw(Scene& scene)
{
	renderWindow.clear();
    for (auto it = scene.GetElementsBegin(); it != scene.GetElementsEnd(); it++)
    {
        SceneObject* sceneObject = static_cast<SceneObject*>(*it);
        RenderComponent* const rc = sceneObject->GetComponent<RenderComponent>();
		TransformComponent* const tc = sceneObject->GetComponent<TransformComponent>();
		const FTransform trans = tc->GetWorldTransform();

		if (!rc || rc->spriteId.empty())
            continue;

		Resource<sf::Texture>* textureRsrc = nullptr;
		if (scene.textureResources.Get(rc->spriteId, textureRsrc))
		{
			sf::Sprite sprite(*textureRsrc->GetAsset());
			sf::FloatRect bounds = sprite.getLocalBounds();
			sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

			sprite.setPosition(trans.Position.X, trans.Position.Y);
			sprite.setRotation(trans.Rotation);
			sprite.setScale(trans.Scale.X, trans.Scale.Y);

			renderWindow.draw(sprite);
		}
	}
	renderWindow.display();
}