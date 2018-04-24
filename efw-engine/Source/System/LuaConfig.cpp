#include "LuaConfig.h"
#include "sol.hpp"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Component/TransformComponent.h"
#include "GameFramework/Component/RenderComponent.h"
#include "GameFramework/Component/LuaComponent.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"

namespace
{
	void RegisterComponentsForObject(sol::table& tbl, SceneObject* obj) 
	{
		tbl.for_each([obj](auto comp_k, auto comp_v) {
			// check for Transform
			if (comp_k.template as<std::string>() == "transform")
			{
				if (comp_v.template is<FTransform>())
				{
					obj->AddComponent<TransformComponent>(comp_v.template as<FTransform>());
				}
				else
				{
					sol::optional<sol::table> transTable = comp_v.template as<sol::table>();
					if (transTable != sol::nullopt)
					{
						sol::optional<FVector> p = transTable->get<FVector>("position");
						sol::optional<float> r = transTable->get<float>("rotation");
						sol::optional<FVector> s = transTable->get<FVector>("scale");
						if (p != sol::nullopt && r != sol::nullopt && s != sol::nullopt)
						{
							obj->AddComponent<TransformComponent>(*p, *r, *s);
						}
					}
					else
					{
						std::cerr << "Error parsing transform component from lua." << std::endl;
					}
				}
			}
			else if (comp_k.template as<std::string>() == "sprite_id")
			{
				obj->AddComponent<RenderComponent>(comp_v.template as<std::string>());
			}
		});
	}
}

void LuaConfig::InitState(sol::state& lua)
{
	lua.open_libraries();

	lua.new_usertype<FVector>("Vector",
							  sol::constructors<void(), void(float, float)>(),
							  "x", &FVector::x,
							  "y", &FVector::y);

	lua.new_usertype<FTransform>("Transform",
								 sol::constructors<void(), void(FVector, float, FVector)>(),
								 "position", &FTransform::position,
								 "rotation", &FTransform::rotation,
								 "scale", &FTransform::scale);

	lua.new_usertype<BaseComponent>("BaseComponent",
									"new", sol::no_constructor,
									"internal_tick", &BaseComponent::Tick);

	lua.new_usertype<LuaComponent>("Component",
								   sol::base_classes, sol::bases<BaseComponent>(),
								   "new", sol::no_constructor,
								   "tick", &LuaComponent::luaTick);

	lua.new_usertype<TransformComponent>("TransformComponent",
										 "new", sol::no_constructor,
										 sol::base_classes, sol::bases<BaseComponent>(),
										 "relative_transform", sol::property(&TransformComponent::SetRelativeTransform, &TransformComponent::GetRelativeTransform),
										 "world_transform", sol::property(&TransformComponent::GetWorldTransform));

	lua.new_usertype<RenderComponent>("RenderComponent",
									  "new", sol::no_constructor,
									  sol::base_classes, sol::bases<BaseComponent>(),
									  "sprite_id", &RenderComponent::spriteId);

	lua.new_usertype<Object>("Object",
							 "name", sol::property(&Object::SetName, &Object::GetName),
							 "new", sol::no_constructor);

	lua.new_usertype<SceneObject>("SceneObject",
								  sol::base_classes, sol::bases<Object>(),
								  "new_component", &SceneObject::Lua_NewComponent,
								  "get_render_comp", &SceneObject::GetComponent<RenderComponent>,
								  "get_transform_comp", &SceneObject::GetComponent<TransformComponent>,
								  "relative_transform", sol::property(&SceneObject::SetRelativeTransform, &SceneObject::GetRelativeTransform),
								  "world_transform", sol::property(&SceneObject::GetWorldTransform),
								  "parent", sol::property(&SceneObject::SetParent, &SceneObject::GetParent),
								  "add_child", &SceneObject::AddChild);

	// half assed scene utype
	lua.new_usertype<Scene>("Scene",
							"add_object", &Scene::AddObject);

	try
	{
		lua.script_file("Scripts/main.lua");
	}
	catch (const sol::error &err)
	{
		std::cerr << err.what();
	}
}

std::unique_ptr<Scene> LuaConfig::GetScene(sol::state& lua, const char* sceneName)
{
	// grab all scenes
	sol::table scenes = lua["scenes"];
	if (!scenes.valid()) { std::cerr << "No scenes found."; return nullptr; }

	if (sceneName == nullptr) sceneName = "main";

	// check if sceneName exists
	sol::optional<sol::table> sceneTbl = scenes[sceneName];
	if (sceneTbl == sol::nullopt) { std::cerr << sceneName << " not found."; return nullptr; }

	Scene *newScene = new Scene();
	newScene->SetSceneName(sceneTbl->get_or<std::string>("name", "untitled"));

	// get entities and iterate through if found
	sol::optional<sol::table> objectsTbl = sceneTbl->get<sol::table>("objects");
	if (objectsTbl != sol::nullopt)
	{
		sceneTbl->set("objects", sol::new_table());
		objectsTbl->for_each([&newScene = newScene, &sceneTbl](auto k, auto v) {
			std::string id = k.template as<std::string>();
			SceneObject *newObject = new SceneObject();

			// get current object's properties
			sol::table objectTbl = v.template as<sol::table>();

			// get object name if available
			newObject->SetName(objectTbl.get_or<std::string>("name", id));

			// get components and iterate through if found
			sol::optional<sol::table> componentsTbl = objectTbl.get<sol::table>("components");
			if (componentsTbl != sol::nullopt)
			{
				RegisterComponentsForObject(componentsTbl.value(), newObject);
			}
			newScene->AddObject(newObject);
			sceneTbl.value()["objects"][id] = newObject;
		});

	return std::unique_ptr<Scene>(newScene);
	}
	return nullptr;
}

bool LuaConfig::BindCallbacks(sol::state& lua, const char* sceneName)
{
	sol::optional<sol::table> scene = lua["scenes"][sceneName];
	if (scene != sol::nullopt)
	{
		sol::function fnReady = scene->get<sol::function>("on_ready");
		if (fnReady.valid()) 
		{
			fnReady();
			return true;
		}
	}
	return false;
}