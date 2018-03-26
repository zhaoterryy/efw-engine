#pragma once
#include "Resource.h"

#include <unordered_map>
#include <iostream>

template <class T>
class ResourceManager
{
public:
    Resource<T>* Get(std::string const& id);
    bool Get(std::string const& id, Resource<T>* outResource);

    void LoadAll();
    void UnloadAll();

    bool Load(std::string const& id);
    bool Unload(std::string const& id);

    void AddResource(std::string const& id, std::string const& path);
private:
    std::unordered_map<std::string, std::unique_ptr<Resource<T>>> resourceMap;
};

template <class T>
Resource<T>* ResourceManager<T>::Get(std::string const& id) 
{
    auto itr = resourceMap.find(id);
	if (itr != resourceMap.end())
		return &*itr->second;

    std::cerr << "Couldn't find resource: " << id;
	return nullptr;
}

template <class T>
bool ResourceManager<T>::Get(std::string const& id, Resource<T>* outResource)
{
	outResource = Get(id);
    if (outResource != nullptr)
        return true;
    else
        return false;
}

template <class T>
void ResourceManager<T>::LoadAll() 
{
    for (auto& itr : resourceMap)
        *itr.second.Load();
}

template <class T>
void ResourceManager<T>::UnloadAll()
{
    for (auto& itr : resourceMap)
        *itr.second.Unload();
}

template <class T>
bool ResourceManager<T>::Load(std::string const& id)
{
    T* resource;
    if (Get(id, &resource))
    {
        resource->Load();
        return true;
    }

    return false;
}

template <class T>
bool ResourceManager<T>::Unload(std::string const& id)
{
    T* resource;
    if (Get(id, &resource))
    {
        resource->Unload();
        return true;
    }
    
    return false;
}

template <class T>
void ResourceManager<T>::AddResource(std::string const& id, std::string const& path) 
{
    if (Get(id) != nullptr)
    {
        std::cerr << "AddResource failed, id already exists." << std::endl;
        return;
    }

	std::cout << "adding resource from " << path;
	resourceMap.emplace(id, std::make_unique<Resource<T>>(path));
}