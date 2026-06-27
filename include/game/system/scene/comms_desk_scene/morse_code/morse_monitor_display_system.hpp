#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/state/settings.hpp"


struct SystemContext;


namespace System::Morse
{

class MonitorDisplay final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, Settings::Morse settings);

private:
	// ------ Functions ------
	static void UpdatePointer(const SystemContext& context, float exitTime);
	static const Component::Morse::Transceiver* GetTransceiver(entt::registry& registry);
	static void SetRegions(entt::registry& registry, Settings::Morse settings, float monitorScale);

};

}