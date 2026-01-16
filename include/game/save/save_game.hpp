#pragma once
#include "entt/entity/fwd.hpp"
#include "game/save/save_result.hpp"


class Game;
struct StatesContext;


namespace Save
{

Result GameToDisk(entt::registry& registry, const StatesContext& context);

}


namespace Load
{

Result GameFromDisk(entt::registry& registry, const StatesContext& context);

}