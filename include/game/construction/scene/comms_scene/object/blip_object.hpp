#pragma once
#include <array>

#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
struct SceneContext;


namespace Object
{

class Blip final
{
public:
    // ------ Types ------
    struct Data final
    {
        entt::entity blip = entt::null;
        entt::entity emitter = entt::null;

        [[nodiscard]] std::array<entt::entity, 2u> All() const noexcept
        {
            return { blip, emitter };
        }
    };


	// ------ Functions ------

	static Data Create(const SceneContext& context, Nc::Vector2f position, int16_t health = 10) noexcept;

};

}