#include "AudioComponent.h"
#include "GameFramework/SceneObject.h"
#include "TransformComponent.h"
#include <SFML/Audio.hpp>

AudioComponent::AudioComponent(SceneObject* inHost, ResourceManager<sf::SoundBuffer>& soundResource) :
    BaseComponent(inHost),
    soundResourceManager(soundResource)
{}

void AudioComponent::Tick(float deltaTime) 
{
    const FVector pos = GetHost()->GetComponent<TransformComponent>()->GetWorldTransform().Position;
    sound.setPosition(pos.X, pos.Y, 0.f);
}

void AudioComponent::PlaySound(std::string id) 
{
	Resource<sf::SoundBuffer>* buffer = nullptr;
    if (soundResourceManager.Get(id, buffer)) 
    {
		sound.setBuffer(*buffer->GetAsset());
        sound.play();
    }
}