#pragma once
#include "entt/entity/entity.hpp"
struct SystemContext;

namespace Component
{
struct Blip;
}

namespace System::Blip
{

class ContactFailure final
{
public:
    static void Initialize(entt::registry& registry, entt::entity entity, Component::Blip& blip);
    static void Update(const SystemContext& context);

};

}
