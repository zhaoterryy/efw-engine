#pragma once

#include "GameFramework/Scene.h"

#include <memory>

namespace sol 
{
    class state;
}

namespace LuaConfig 
{
    void InitState(sol::state& lua);
    std::unique_ptr<Scene> GetScene(sol::state& lua, const char* sceneName = nullptr);
    bool BindCallbacks(sol::state& lua, const char* sceneName);
};