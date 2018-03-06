#pragma once

#include "Resource.h"

#include <unordered_map>

template <class T>
class ResourceManager
{
public:
    T& Get(std::string const& id);
    bool Get(std::string const& id, T& outResource);

    void LoadAll();
    void UnloadAll();

    bool Load(std::string const& id);
    bool Unload(std::string const& id);

private:
    std::unordered_map<std::string, std::unique_ptr<Resource<T>>> resourceMap;
};

template <class T>
T& ResourceManager<T>::Get(std::string const& id) 
{
    auto itr = resourceMap.find(id);
    if (itr != resourceMap.end()) 
        return *itr->second;

    std::cerr << "Couldn't find resource: " << id;
    return &Resource<T>::NULL_RESOURCE;
}

template <class T>
bool ResourceManager<T>::Get(std::string const& id, T& outResource)
{
    outResource = Get(id);
    if (outResource != Resource<T>::NULL_RESOURCE)
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