#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>
#include <iostream>

template <class T>
class Resource
{
public:
	Resource(std::string const& path);

	void Load();
	void Unload();

	bool IsLoaded() const;

	T* GetAsset() const;

	std::string filePath;

private:
	std::unique_ptr<T> obj;
	bool isLoaded;
};

template <class T>
Resource<T>::Resource(std::string const& path) :
	filePath(path),
	isLoaded(false),
	obj(std::make_unique<T>())
{
	Load();
}

template <>
inline void Resource<sf::Texture>::Load()
{
	if (obj->loadFromFile(filePath))
	{
		isLoaded = true;
		return;
	}
	
	std::cerr << "Unable to load texture from: " << filePath;
	isLoaded = false;
}

template <>
inline void Resource<sf::SoundBuffer>::Load()
{
	if (obj->loadFromFile(filePath))
	{
		isLoaded = true;
		return;
	}
	std::cerr << "Unable to load sound buffer from: " << filePath;
	isLoaded = false;
}

template <>
inline void Resource<sf::Font>::Load()
{
	if (obj->loadFromFile(filePath))
	{
		isLoaded = true;
		return;
	}
	std::cerr << "Unable to load font from: " << filePath;
	isLoaded = false;
}

template <class T>
inline void Resource<T>::Unload()
{
	obj.reset();
	isLoaded = false;
}

template <class T>
inline bool Resource<T>::IsLoaded() const
{
	return isLoaded;
}

template <class T>
inline T* Resource<T>::GetAsset() const
{
	if (isLoaded)
		return &*obj;
	else
		return nullptr;
}