#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;


namespace Nc
{
class ResourceStore;
struct RenderContext;
};


struct BuildContext final
{
    // ------ Members ------
    entt::registry& registry;
    Nc::ResourceStore& store;
    Nc::RenderContext& renderContext;
    GameState& game;


    // ------ Constructors ------

    BuildContext(
        entt::registry& registry,
        Nc::ResourceStore& store,
        Nc::RenderContext& renderContext,
        GameState& game
    ) noexcept
        : registry(registry),
          store(store),
          renderContext(renderContext),
          game(game) { }
};
