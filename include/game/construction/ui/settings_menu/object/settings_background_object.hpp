#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;


namespace Object
{

class SettingsBackground final
{
public:
    static void Create(entt::registry& registry, GameState& gameState, Nc::Vector2f windowSize) noexcept;

private:
    class FrontTexture final
    {
    public:
        static entt::entity Create(entt::registry& registry, GameState& gameState) noexcept;

    };


    class Backdrop final
    {
    public:
        static entt::entity Create(entt::registry& registry, Nc::Vector2f windowSize) noexcept;

    };

};

}
