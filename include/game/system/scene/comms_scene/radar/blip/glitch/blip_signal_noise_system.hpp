#pragma once
#include "entt/entity/registry.hpp"
struct SystemContext;

namespace Component {
namespace Glitch {
struct SignalNoise;
}

struct Blip;
}


namespace System::Blip
{

class TextError final
{
public:
    // ------ Functions ------
    static void Initialize(entt::registry& registry, entt::entity entity, Component::Blip& blip);
    static void Update(const SystemContext& context);

private:
    // ------ Functions ------
    static void UpdateCharacters(const SystemContext& context, Component::Glitch::SignalNoise& error);

};

}
