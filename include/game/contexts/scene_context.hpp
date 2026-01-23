#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;


namespace Nc
{
class ResourceStore;
};


struct SceneContext final
{
    // ------ Members ------
    entt::registry& registry;
    Nc::ResourceStore& store;
    GameState& game;


    // ------ Constructors ------

    SceneContext(entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState) noexcept
        : registry(registry), store(resourceStore), game(gameState) {}
};
