#pragma once
#include "components/core/sound_emitter_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.h"
#include <string>
class ResourceStore;


namespace Construct
{
	entt::entity SceneBackgroundEntity(
		Texture2D&& texture, entt::registry& registry, Scene scene
	);

	entt::entity SoundEntity(
		const std::string& filePath, entt::registry& registry,
		Scene scene, Nc::Vector2f position, 
		bool playOnStart = false, bool loops = false
	);

	Component::SoundEmitter& AddSound(
		const std::string& filePath, entt::entity entity, entt::registry& registry,
		bool playOnStart = false, bool loops = false
	);
}