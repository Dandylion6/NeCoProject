#include "game/system/scene/comms_desk_scene/morse_code/morse_monitor_display_system.hpp"

#include "core/math/vector_math.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/settings.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"
#include "game/utility/morse_code.hpp"


void System::Morse::MonitorDisplay::Update(const SystemContext& context, const Settings::Morse settings)
{
    constexpr float SMOOTH_SPEED = 32.0f;
    constexpr float MIN_ROTATION = -90.0f;

    const entt::entity gaugeEntity = entt::get_single<Tag::Morse::Gauge>(context.registry);
    const entt::entity transceiverEntity = entt::get_single<Component::Morse::Transceiver>(context.registry);

    const auto& transceiver = context.registry.get<Component::Morse::Transceiver>(transceiverEntity);
    auto& transform = context.registry.get<Component::Transform>(gaugeEntity);

    if (transceiver.isPushed)
    {
        constexpr float MAX_ROTATION = 84.0f;

        const float time = transceiver.intervalSeconds / MorseCode::ExitTime(settings.dotSeconds);
        transform.rotation = Nc::Math::Lerp(MIN_ROTATION, MAX_ROTATION, time);
        return;
    }

    transform.rotation = Nc::Math::SmoothApproach(
        transform.rotation,
        MIN_ROTATION,
        context.deltaTime,
        SMOOTH_SPEED
    );
}