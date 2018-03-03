#pragma once
#include "SFML/Graphics.hpp"

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		TestTexture1
	};
}

namespace Shaders
{
	enum ID
	{
		
	};
}

namespace Fonts
{
	enum ID
	{
		
	};
}

namespace SoundEffect
{
	enum ID
	{
		
	};
}

namespace Music
{
	enum ID
	{
		
	};
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>			TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>					FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID>				ShaderHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;

