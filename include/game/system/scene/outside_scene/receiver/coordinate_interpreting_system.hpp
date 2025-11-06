#pragma once
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
namespace Nc { class ResourceStore; };


struct CoordResult
{
	enum Axis: uint8_t { Invalid, Horizontal, Vertical };
	
	int16_t coordinateLength = 0;
	Axis axis = Invalid;
	bool isValid = false;
};


class CoordinateInterpretingSystem
{
public:
	static const std::string COMMAND;

	static CoordResult InterpretMessageAsCoord(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

    static void ConfirmCoordinateCommand(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver
	);

};