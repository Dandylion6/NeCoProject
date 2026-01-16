#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
struct SystemContext;


namespace Nc
{
class Random;
}

namespace Component
{
struct Transform;
struct Blip;
}


namespace System::Blip
{
class Jumble final
{
public:
    // ------ Functions ------
    static void Initialize(entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability);
    static void Update(const SystemContext& context);

private:
    // ------ Functions ------
    static Nc::Vector2i GetDisplayPosition(const Component::Transform& transform, Component::Glitch::Distortion jumble);
    static Component::Glitch::Distortion GenerateRandomJumble(Nc::Random& randomService, float stability);
};
}
