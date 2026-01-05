#pragma once
#include "entt/entity/fwd.hpp"


namespace Entity
{

class SettingsHeader final
{
public:
    static const entt::entity Create(entt::registry& registry) noexcept;

};


class GameplaySettingsHeader final
{
public:
    static const entt::entity Create(entt::registry& registry) noexcept;

};

}