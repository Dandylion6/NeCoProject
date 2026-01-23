#pragma once
#include <string>

#include "entt/entity/fwd.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
struct SystemContext;


namespace Nc
{
class ResourceStore;
};


struct CoordResult final
{
	enum Axis: uint8_t { Invalid, Horizontal, Vertical };

	int16_t coordinateLength = 0;
	Axis axis = Invalid;
	bool isValid = false;
};


namespace System::Receiver::Interpret
{
class Coordinate final
{
public:
	static CoordResult MessageAsCoord(const std::string& message);
	static void ConfirmCommand(const SystemContext& context, Component::Receiver& receiver);

private:
	static bool TryParseSuffix(char suffix, CoordResult& result);
};
}

