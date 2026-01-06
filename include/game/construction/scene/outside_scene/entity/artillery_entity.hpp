#pragma once
#include "entt/entity/fwd.hpp"

namespace Entity
{

class Artillery final
{
public:
    static entt::entity Create(entt::registry& registry) noexcept;

};

};