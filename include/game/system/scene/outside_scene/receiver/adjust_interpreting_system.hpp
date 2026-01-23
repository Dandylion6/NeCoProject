#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_coordinate_system.hpp"
#include <string>
struct SystemContext;

namespace Nc { class ResourceStore; };


namespace System::Receiver::Interpret
{

class Adjust final
{
public:
	// ------ Members ------
    static constexpr char COMMAND[] = "ADJ";


	// ------ Functions ------

	static void HandleMessage(const SystemContext& context, Component::Receiver& receiver, const std::string& message);

private:
	// ------ Functions ------
    static void ConfirmCommand(const SystemContext& context, Component::Receiver& receiver);
	static void AdjustArtillery(entt::registry& registry, CoordResult result);
    
};

}
