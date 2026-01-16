#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"


struct SystemContext;


namespace System::Blip
{

class Glitch final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);
	static float GenerateGlitchDuration(entt::registry& registry, float stability);

};

}

