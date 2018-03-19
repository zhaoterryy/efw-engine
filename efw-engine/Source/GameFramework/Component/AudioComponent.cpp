#include "AudioComponent.h"
#include "GameFramework/SceneObject.h"
#include "TransformComponent.h"
#include <SFML/Audio.hpp>

AudioComponent::AudioComponent(SceneObject* inHost, ResourceManager<sf::SoundBuffer>& soundResource) :
    BaseComponent(inHost),
    soundResourcesRef(soundResource)
{}

void AudioComponent::Tick(float deltaTime) 
{
    const FVector pos = GetHost()->GetComponent<TransformComponent>()->GetWorldTransform().Position;
    sound.setPosition(pos.X, pos.Y, 0.f);
}

void AudioComponent::PlaySound(std::string id) 
{
    sf::SoundBuffer buffer;
    if (soundResourcesRef.Get(id, buffer)) 
    {
        sound.setBuffer(buffer);
        sound.play();
    }
}