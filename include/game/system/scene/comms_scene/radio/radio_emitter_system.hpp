#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
struct Sound;
struct SystemContext;


namespace System::Radio
{

class Emitter final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);
	static void Broadcast(entt::registry& registry, const Sound& sound, BroadcastPriority priority);

};

}