#include "game/construction/shared/entity/environment/runtime_audio_entity.hpp"

#include "core/runtime/resource_store.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


entt::entity Entity::RuntimeAudio::Create(
Nc::ResourceStore& store,
    entt::registry& registry,
    const std::string& sound,
    const Component::Transform& transform,
    float volume,
    bool isSpacial,
    bool ignoreScene
) noexcept
{

    const entt::entity entity = registry.create();

    registry.emplace<Component::Transform>(entity, transform);
    registry.emplace<Component::ShotAudio>(entity, store.CreateSoundHandle(sound), true);
    Component::AudioEmitter& emitter = registry.emplace<Component::AudioEmitter>(entity, isSpacial, volume, ignoreScene);
    System::Audio::Emitter::PlayEmitter(emitter);

    return entity;
}
