#include "game/construction/scene/comms_desk_scene/object/morse_monitor_object.hpp"

#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/state/scene.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"


void Object::MorseMonitor::Create(const SceneContext& context) noexcept
{
    constexpr auto POSITION = Nc::Vector2f(284.0f, 135.0f);
    constexpr auto OFFSET = Nc::Vector2f(25.0f, 68.0f);
    constexpr char GAUGE_ALBEDO_PATH[] = "assets/environment/objects/morse_transmitter/morse_timing_knob/morse_transmission_gauge_albedo.png";
    constexpr char GAUGE_NORMAL_PATH[] = "assets/environment/objects/morse_transmitter/morse_timing_knob/morse_transmission_gauge_normal.png";
    constexpr char GAUGE_AO_PATH[] = "assets/environment/objects/morse_transmitter/morse_timing_knob/morse_transmission_gauge_ao.png";

    const entt::entity entity = context.registry.create();

    context.registry.emplace<Tag::Morse::Gauge>(entity);

    const Texture2D& albedoTexture = context.store.GetTexture(GAUGE_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(GAUGE_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(GAUGE_AO_PATH);
    context.registry.emplace<Component::Sprite>(entity, albedoTexture, normalTexture, aoTexture);

    const auto size = Nc::Vector2f(albedoTexture.width, normalTexture.height);
    context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, size, OFFSET);
}