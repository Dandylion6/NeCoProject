#include "game/system/scene/comms_scene/radar/blip/glitch/blip_contact_failure_system.hpp"

#include "core/math/random.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Blip::ContactFailure::Initialize(
    entt::registry& registry,
    const entt::entity entity,
    Component::Blip& blip
)
{
    blip.state = Component::Blip::CompleteFailure;
    registry.emplace<Component::Glitch::ContactFailure>(entity);
}


void System::Blip::ContactFailure::Update(const SystemContext& context)
{
    auto& randomService = context.registry.ctx().get<Nc::Random>();

    const auto view = context.registry.view<Component::Blip, Component::Text, Component::Glitch::ContactFailure>();
    for (auto [entity, blip, text, failure] : view.each())
    {
        constexpr float RANDOM_OFFSET = Component::Glitch::ContactFailure::OFFSET_RANGE;
        constexpr Nc::Vector2f RANGE = Component::Glitch::ContactFailure::GLITCH_INTERVAL_RANGE;

        if (blip.remainingGlitchSeconds <= 0.0f)
        {
            blip.state = Component::Blip::Stable;
            context.registry.remove<Component::Glitch::ContactFailure>(entity);
            continue;
        }

        text.text = "???";
        if (failure.offsetSecondsLeft <= 0.0f) continue;

        failure.offsetSecondsLeft = randomService.RangeFloat(RANGE.x, RANGE.y);

        Nc::Vector2f newGlitchOffset = Nc::Vector2f::Zero();
        newGlitchOffset.x = randomService.RangeFloat(-RANDOM_OFFSET, RANDOM_OFFSET);
        newGlitchOffset.y = randomService.RangeFloat(-RANDOM_OFFSET, RANDOM_OFFSET);
        failure.glitchedOffset = newGlitchOffset;
    }
}
