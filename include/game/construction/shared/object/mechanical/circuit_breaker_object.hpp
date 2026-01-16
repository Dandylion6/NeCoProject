#pragma once
#include <array>

#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"


namespace Component::Logic { struct CircuitBreaker; }
namespace Component { struct Transform; }


namespace Object
{

class CircuitBreaker final
{
public:
    // ------ Types ------
    struct Data final
    {
        Component::Logic::CircuitBreaker& breaker;
        entt::entity entity = entt::null;
        entt::entity indicator = entt::null;

        [[nodiscard]] std::array<entt::entity, 2u> All() const noexcept { return { entity, indicator }; }
    };


    // ------- Functions ------

    static Data Create(entt::registry& registry, const Component::Transform& transform, entt::entity system) noexcept;

private:
    class Indicator final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(entt::registry& registry, Scene scene, Nc::Vector2f breakerCenterPosition) noexcept;

    };

};

}
