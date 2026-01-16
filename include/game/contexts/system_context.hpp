#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;


namespace Nc
{
class ResourceStore;
}


struct SystemContext final
{
    // ------ Members ------
    entt::registry& registry;
    Nc::ResourceStore& store;
    GameState& game;
    float deltaTime;


    // ------ Constructors ------

    SystemContext(entt::registry& registry, Nc::ResourceStore& store, GameState& game, const float deltaTime) noexcept
        : registry(registry), store(store), game{ game }, deltaTime{ deltaTime } { };
};
