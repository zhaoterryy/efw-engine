#pragma once

#include "System/ResourceManager.h"
#include <SFML/Audio/Sound.hpp>
#include "BaseComponent.h"
#include <string>

namespace sf 
{
	class SoundBuffer;
}

class AudioComponent : public BaseComponent 
{
public:
	AudioComponent(SceneObject* inHost);

	virtual void Tick(float deltaTime) override;
	void PlaySound(std::string id);

	bool IsPlaying() const;

private:
	ResourceManager<sf::SoundBuffer>* soundResourceManager;
	sf::Sound sound;
};
