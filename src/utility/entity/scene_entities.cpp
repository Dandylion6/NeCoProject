#include "components/core/rendering/sprite_component.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/entity/scene_entities.h"
#include "utility/vector2.h"
#include <string>


entt::entity Construct::SceneBackgroundEntity(
	Texture2D&& texture, entt::registry& registry, Scene scene
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f offset = size * 0.5f;
	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;

	registry.emplace<Component::Transform>(entity, scene, position, size, offset);
	registry.emplace<Component::Sprite>(entity, texture);
	return entity;
}


entt::entity Construct::SoundEntity(
	const std::string& filePath, entt::registry& registry,
	Scene scene, Nc::Vector2f position, bool playOnStart, bool loops
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, scene, position);
	Component::SoundEmitter& emitter = registry.emplace<Component::SoundEmitter>(entity, LoadSound(filePath.c_str()), loops);
	emitter.duration = static_cast<float>(emitter.sound.frameCount) / emitter.sound.stream.sampleRate;
	emitter.loops = loops;
	emitter.isPlaying = playOnStart;
	return entity;
}

Component::SoundEmitter& Construct::AddSound(
	const std::string& filePath, entt::entity entity, entt::registry& registry, 
	bool playOnStart, bool loops
)
{
	Component::SoundEmitter& emitter = registry.emplace<Component::SoundEmitter>(entity, LoadSound(filePath.c_str()), loops);
	emitter.duration = static_cast<float>(emitter.sound.frameCount) / emitter.sound.stream.sampleRate;
	emitter.loops = loops;
	emitter.isPlaying = playOnStart;
	return emitter;
}
