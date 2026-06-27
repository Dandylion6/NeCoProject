#include "game/construction/scene/comms_desk_scene/entity/morse_transceiver_entity.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


entt::entity Entity::MorseTransceiver::Create(const SceneContext& context) noexcept
{
    constexpr char MORSE_TONE_PATH[] = "assets/audio/object/morse_tone.wav";
    constexpr char HANDLE_ALBEDO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_albedo.png";
    constexpr char HANDLE_NORMAL_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_normal.png";
    constexpr char HANDLE_AO_PATH[] = "assets/environment/objects/morse_transmitter/morse_handle/morse_handle_ao.png";
    constexpr auto SIZE = Nc::Vector2f::Scale(48.0f);
    constexpr auto OFFSET = Nc::Vector2f(0.0f, 143.0f);

    const entt::entity entity = context.registry.create();

    context.registry.emplace<Component::Morse::Transceiver>(entity);

    const Texture2D& albedoTexture = context.store.GetTexture(HANDLE_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(HANDLE_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(HANDLE_AO_PATH);

    context.registry.emplace<Component::Sprite>(entity, albedoTexture, normalTexture, aoTexture);

    context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, SIZE);
    context.registry.emplace<Component::Action::Input>(entity, KEY_SPACE);
    context.registry.emplace<Component::Action::Click>(entity, OFFSET);

    const Music& morseTone = context.store.GetMusic(MORSE_TONE_PATH);
    context.registry.emplace<Component::LoopedAudio>(entity, morseTone);

    auto& emitter = context.registry.emplace<Component::AudioEmitter>(entity, 0.0f);
    System::Audio::Emitter::PlayEmitter(emitter);

    return entity;
}
