#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"


struct SceneContext;


namespace Entity
{

class MorseTransceiver final
{
public:
    // ------ Members ------

    static constexpr auto POSITION = Nc::Vector2f(93.0f, 532.0f);
    static constexpr auto PUSHED_POSITION = Nc::Vector2f(95.0f, 532.0f) ;


    // ------ Functions ------

    static entt::entity Create(const SceneContext& context) noexcept;

};

}
