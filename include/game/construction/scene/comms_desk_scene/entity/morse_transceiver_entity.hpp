#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"


struct SceneContext;


namespace Entity
{

class MorseTransceiver final
{
public:
    static constexpr auto POSITION = Nc::Vector2f(139.0f, 551.0f);
    static constexpr auto PUSHED_POSITION = Nc::Vector2f(144.0f, 552.0f) ;

    static entt::entity Create(const SceneContext& context) noexcept;

};

}
