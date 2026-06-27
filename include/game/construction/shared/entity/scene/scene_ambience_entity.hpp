#pragma once
#include <string>

#include "entt/entity/entity.hpp"
#include "game/state/scene.hpp"


struct SceneContext;


namespace Entity
{

class SceneAmbience final
{
public:
    // ------ Functions ------
    static entt::entity Create(const SceneContext& context, const std::string& audioPath, Scene boundScene);

};

}
