#pragma once
#include <string>

#include "entt/entity/fwd.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_coordinate_system.hpp"
struct SystemContext;


namespace Component
{
struct Receiver;
}


namespace Nc
{
class ResourceStore;
}


namespace System::Receiver::Interpret
{

class Aiming final
{
public:
	// ------ Members ------
	static constexpr char COMMAND[] = "AIM";


	// ------ Functions ------

	static void HandleMessage(const SystemContext& context, Component::Receiver& receiver, const std::string& message);

private:
	// ------ Functions ------
	static void ConfirmCommand(const SystemContext& context, Component::Receiver& receiver);
	static void SetArtilleryTarget(entt::registry& registry, CoordResult result);

};

};
