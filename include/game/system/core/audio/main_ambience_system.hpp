#pragma once
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
struct SystemContext;


namespace Nc
{
class ResourceStore;
};


namespace Component
{
struct Transform;
struct LoopedAudio;
}


namespace System::Audio
{

class MainAmbience final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);

private:
	// ------ Types ------
	struct Context final
	{
		Nc::ResourceStore& store;
		Component::LoopedAudio& emitter;
		entt::registry& registry;
	};


	// ------ Functions ------

	static void TryPlayAmbience(const Context& context, Component::Transform& transform, Scene scene);
	static void TransitionAmbientAudio(const Context& context, const std::string& filePath);

};

}

