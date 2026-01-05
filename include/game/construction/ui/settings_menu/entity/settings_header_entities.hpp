#pragma once
#include "entt/entity/fwd.hpp"


namespace Construct
{
    const entt::entity SettingsHeaderEntity(entt::registry& registry);

    const entt::entity GameplaySettingsHeaderEntity(entt::registry& registry);

}


namespace Entity
{

class SettingsHeader final
{
public:
    static const entt::entity Create(
        entt::registry& registry
    ) noexcept;

};


class GameplaySettingsHeader final
{
public:
    static const entt::entity Create(
        entt::registry& registry
    ) noexcept;

};

}