#include "Renderer.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"
#include "GameFramework/RenderComponent.h"

void Renderer::Draw(Scene& scene)
{
    for (auto it = scene.GetElementsBegin(); it != scene.GetElementsEnd(); it++)
    {
        RenderComponent* const rc = it->GetComponent<RenderComponent>();
        if (rc == nullptr || rc->id.empty())
            continue;


    }
}