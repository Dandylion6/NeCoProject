#include "game/construction/shared/entity/scene/scene_ambience_entity.hpp"

#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


entt::entity Entity::SceneAmbience::Create(const SceneContext& context, const std::string& audioPath, const Scene boundScene)
{
    const entt::entity entity = context.registry.create();

    context.registry.emplace<Component::Transform>(entity, boundScene, Nc::Vector2f(Nc::RENDER_RESOLUTION * 0.5f));
    context.registry.emplace<Component::AudioModifier>(entity);

    const Music& music = context.store.GetMusic(audioPath);
    auto& audio = context.registry.emplace<Component::LoopedAudio>(entity, music);
    System::Audio::Emitter::PlayEmitter(audio);

    return entity;
}
