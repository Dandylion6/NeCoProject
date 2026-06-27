#include "game/system/scene/comms_desk_scene/morse_code/morse_monitor_display_system.hpp"

#include <cmath>

#include "core/math/vector_math.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/construction/scene/comms_desk_scene/object/morse_monitor_object.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/settings.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"
#include "game/utility/morse_code.hpp"


void System::Morse::MonitorDisplay::Update(const SystemContext& context, const Settings::Morse settings)
{
    const float monitorScale = Object::MorseMonitor::GAUGE_SIZE.y / MorseCode::ExitTime(settings.dotSeconds);
    UpdatePointer(context, MorseCode::ExitTime(settings.dotSeconds));
    SetRegions(context.registry, settings, monitorScale);
}


void System::Morse::MonitorDisplay::UpdatePointer(const SystemContext& context, const float exitTime)
{
    constexpr Nc::Vector2f MAX_POSITION = Object::MorseMonitor::POSITION;
    constexpr float DOWN_ROTATION = Object::MorseMonitor::ROTATION + 90.0f;

    const entt::entity entity = entt::get_single<Tag::Morse::Monitor>(context.registry);
    auto& transform = context.registry.get<Component::Transform>(entity);

    const Component::Morse::Transceiver* result = GetTransceiver(context.registry);
    if (result == nullptr) return;

    const Nc::Vector2f direction = Nc::Vector2f(
        std::cos(DOWN_ROTATION * Nc::Math::DEG_TO_RAD),
        std::sin(DOWN_ROTATION * Nc::Math::DEG_TO_RAD)
    );

    const Nc::Vector2f originPosition = Nc::Vector::Round(MAX_POSITION + direction * Object::MorseMonitor::GAUGE_SIZE.y);

    const Component::Morse::Transceiver& transceiver = *result;
    if (!transceiver.isPushed)
    {
        constexpr float SMOOTH_SPEED = 48.0f;

        transform.position = Nc::Vector::SmoothApproach(
            transform.position,
            originPosition,
            context.deltaTime,
            SMOOTH_SPEED
        );
        return;
    }

    const float time = transceiver.intervalSeconds / exitTime;
    transform.position = Nc::Vector::Lerp(originPosition, MAX_POSITION, time);
}


const Component::Morse::Transceiver* System::Morse::MonitorDisplay::GetTransceiver(entt::registry& registry)
{
    const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(registry);
    if (entity == entt::null) return nullptr;
    return &registry.get<Component::Morse::Transceiver>(entity);
}


void System::Morse::MonitorDisplay::SetRegions(
    entt::registry& registry,
    const Settings::Morse settings,
    const float monitorScale
)
{
    constexpr float DOWN_ROTATION = Object::MorseMonitor::ROTATION + 90.0f;
    constexpr Nc::Vector2f ORIGIN = Object::MorseMonitor::POSITION;
    constexpr float X_OFFSET = Object::MorseMonitor::GAUGE_SIZE.x * 0.5f;

    const float marginWidth = MorseCode::ErrorMargin(settings.dotSeconds) * monitorScale;
    const Nc::Vector2f downDirection = Nc::Vector2f(
        std::cos(DOWN_ROTATION * Nc::Math::DEG_TO_RAD),
        std::sin(DOWN_ROTATION * Nc::Math::DEG_TO_RAD)
    );
    const Nc::Vector2f rightDirection = Nc::Vector2f(
        std::cos(Object::MorseMonitor::ROTATION * Nc::Math::DEG_TO_RAD),
        std::sin(Object::MorseMonitor::ROTATION * Nc::Math::DEG_TO_RAD)
    );


    const auto view = registry.view<Component::Morse::MonitorRegion, Component::Transform>();
    for (auto [entity, region, transform] : view.each())
    {
        float pulseTime = 0.0f;
        switch (region.region)
        {
        case Component::Morse::MonitorRegion::Dot:
            pulseTime = settings.dotSeconds;
            break;
        case Component::Morse::MonitorRegion::Dash:
            pulseTime = MorseCode::DashTime(settings.dotSeconds);
            break;
        }

        Nc::Vector2f position = ORIGIN + downDirection * monitorScale * pulseTime;
        position += rightDirection * X_OFFSET;

        transform.offset.y = std::roundf(marginWidth);
        transform.size.y = std::roundf(marginWidth * 2.0f);
        transform.position = Nc::Vector::Round(position);
    }
}
