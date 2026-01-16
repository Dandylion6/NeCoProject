#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct SystemContext;


namespace Component
{
struct Audio;
struct LoopedAudio;
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
		Component::Transform& transform;
		float deltaTime;
		entt::entity entity;
	};


	// ------ Functions ------

	static void UpdateEmitter(const Context& context, const Component::Audio& emitter);
	static void UpdateLoopedEmitter(const Context& context, const Component::LoopedAudio& emitter);
	static float GetPan(Nc::Vector2f position);

};

}

