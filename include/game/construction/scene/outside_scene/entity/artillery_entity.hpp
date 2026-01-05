#pragma once
#include "entt/entity/fwd.hpp"

namespace Entity
{

class Artillery
{
public:
    static const entt::entity Create(
        entt::registry& registry
    ) noexcept;

};

};