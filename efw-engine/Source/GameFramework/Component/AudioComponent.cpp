#include "AudioComponent.h"
#include "GameFramework/SceneObject.h"
#include "GameFramework/Scene.h"
#include "TransformComponent.h"
#include <SFML/Audio.hpp>

AudioComponent::AudioComponent(SceneObject* inHost) :
    BaseComponent(inHost)
{
}

void AudioComponent::Tick(float deltaTime) 
{
//     const FVector pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldTransform().position;
//     sound.setPosition(pos.x, pos.y, 0.f);
}

void AudioComponent::PlaySound(std::string id) 
{
	if (IsPlaying())
		return;

	soundResourceManager = &GetOwner()->GetScene()->soundResources;
	Resource<sf::SoundBuffer>* buffer = nullptr;
    if (soundResourceManager->Get(id, buffer)) 
    {
		sound.setBuffer(*buffer->GetAsset());
        sound.play();
    }
}

bool AudioComponent::IsPlaying() const
{
	return sound.getStatus() == sf::SoundSource::Playing;
}
