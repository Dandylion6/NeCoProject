#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/core/audio/audio_modifier_component.hpp"
#include "game/state/scene.hpp"
struct SystemContext;


namespace Component
{
struct LoopedAudio;
struct Audio;
struct Transform;
}


namespace System::Audio
{

class Emitter final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);
	static void PlayEmitter(const Component::Audio& emitter);
	static void PlayEmitter(const Component::LoopedAudio& emitter);
	static void StopEmitter(const Component::Audio& emitter);
	static void StopEmitter(const Component::LoopedAudio& emitter);

private:
	// ------ Types ------
	struct Context final
	{
	    entt::registry& registry;
		Component::Transform& transform;
		float deltaTime;
		entt::entity entity;
	};


	// ------ Functions ------

	static void UpdateEmitter(Scene currentScene, const Context& context, const Component::Audio& emitter);
	static void UpdateLoopedEmitter(Scene currentScene, const Context& context, const Component::LoopedAudio& emitter);
	static Component::AudioModifier GetAudioModifier(Scene currentScene, const Context& context);

};

}

