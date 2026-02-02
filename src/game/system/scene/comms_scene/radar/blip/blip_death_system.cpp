#include "game/system/scene/comms_scene/radar/blip/blip_death_system.hpp"

#include <vector>

#include "core/data/tween.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/shared/stat/health_component.hpp"


void System::Blip::Death::Update(entt::registry& registry)
{
    std::vector<entt::entity> toKill{ };
    const auto view = registry.view<const Component::Health, Component::Blip, Component::TweenCollection>();
    for (auto [entity, health, blip, collection] : view.each())
    {
        if (health.health > 0) continue;
        blip.isActive = false;

        const Nc::Tween& fadeIn = collection.tweens.at(Component::Blip::BlipFadeIn);
        const Nc::Tween& fadeOut = collection.tweens.at(Component::Blip::BlipFadeOut);
        if (fadeIn.state != Nc::Tween::Completed && fadeOut.state != Nc::Tween::Completed)
            toKill.push_back(entity);
    }

    for (const entt::entity entity : toKill)
        registry.destroy(entity);
}
