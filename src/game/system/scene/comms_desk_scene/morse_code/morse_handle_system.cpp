#include "game/system/scene/comms_desk_scene/morse_code/morse_handle_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/construction/scene/comms_desk_scene/entity/morse_transceiver_entity.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/contexts/system_context.hpp"


void System::Morse::Handle::Update(const SystemContext& context) noexcept
{
    constexpr char HANDLE_ALBEDO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_albedo.png";
    constexpr char HANDLE_NORMAL_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_normal.png";
    constexpr char HANDLE_AO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_ao.png";

    constexpr char HANDLE_PUSHED_ALBEDO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_pushed_albedo.png";
    constexpr char HANDLE_PUSHED_NORMAL_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_pushed_normal.png";
    constexpr char HANDLE_PUSHED_AO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_pushed_ao.png";

    const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(context.registry);
    const auto& transceiver = context.registry.get<Component::Morse::Transceiver>(entity);

    if (!transceiver.inputJustChanged) return; // No need to update the visuals.

    auto& sprite = context.registry.get<Component::Sprite>(entity);
    auto& transform = context.registry.get<Component::Transform>(entity);

    if (transceiver.isPushed)
    {
        transform.position = Entity::MorseTransceiver::PUSHED_POSITION;

        const Texture2D& albedoTexture = context.store.GetTexture(HANDLE_PUSHED_ALBEDO_PATH);
        const Texture2D& normalTexture = context.store.GetTexture(HANDLE_PUSHED_NORMAL_PATH);
        const Texture2D& aoTexture = context.store.GetTexture(HANDLE_PUSHED_AO_PATH);

        sprite.albedo = albedoTexture;
        sprite.normals = normalTexture;
        sprite.ambientOcclusion = aoTexture;
        return;
    }

    transform.position = Entity::MorseTransceiver::POSITION;

    const Texture2D& albedoTexture = context.store.GetTexture(HANDLE_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(HANDLE_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(HANDLE_AO_PATH);

    sprite.albedo = albedoTexture;
    sprite.normals = normalTexture;
    sprite.ambientOcclusion = aoTexture;
}
