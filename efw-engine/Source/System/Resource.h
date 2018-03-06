#pragma once
#include "SFML/Graphics/Texture.hpp"
#include <memory>
#include <string>

template <class T>
class Resource
{
public:
	static Resource* NULL_RESOURCE;

	Resource(std::string const& path);

	void Load();
	void Unload();

	void SetFilePath(std::string const& path);
	std::string GetFilePath() const;

	bool IsLoaded() const;

	T& Get() const;
private:
	std::string filePath;
	std::unique_ptr<T> obj;
	bool isLoaded;
};

template <class T>
Resource<T>* Resource<T>::NULL_RESOURCE = nullptr;

template <class T>
Resource<T>::Resource(std::string const& path) :
	filePath(path),
	isLoaded(false)
{	
}

template <>
Resource<sf::Texture>::Resource(std::string const& path) :
	filePath(path)
{
	if (obj->loadFromFile(path))
		isLoaded = true;

	std::cerr << "Unable to load texture from: " << path;
	isLoaded = false;	
}

template <>
void Resource<sf::Texture>::Load() 
{
	obj->loadFromFile(filePath);
	isLoaded = true;
}

template <class T>
void Resource<T>::Unload()
{
	obj.reset();
	isLoaded = false;
}

template <class T>
void Resource<T>::SetFilePath(std::string const& path)
{
	filePath = path;
}

template <class T>
std::string Resource<T>::GetFilePath() const 
{
	return filePath;
}

template <class T>
bool Resource<T>::IsLoaded() const
{
	return isLoaded;
}

template <class T>
T& Resource<T>::Get() const
{
	if (obj)
		return *obj;
	else
		return &NULL_RESOURCE;
}
