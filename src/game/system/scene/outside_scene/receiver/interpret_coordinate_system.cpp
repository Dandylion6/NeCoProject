#include "game/system/scene/outside_scene/receiver/interpret_coordinate_system.hpp"

#include <string>

#include "raylib.h"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"


CoordResult System::Receiver::Interpret::Coordinate::MessageAsCoord(const std::string& message)
{
	constexpr size_t MINIMUM_COORD_LENGTH = 2u;

	CoordResult result{ };

	// A coordinate message would at least require a suffix and digit.
	if (message.length() < MINIMUM_COORD_LENGTH) return result;

	if (!TryParseSuffix(message.back(), result)) return result;

	size_t startingIndex = 0u;
	int sign = 1;
	if (message.front() == 'N')
	{
		sign = -1;
		++startingIndex;
	}

	// Check if there are any digits, if not don't assume it is a coordinate command.
	int length = 0;
	bool hasDigit = false;
	for (size_t i = startingIndex; i < message.length() - 1u; ++i)
	{
		const char character = message.at(i);
		if (!MorseCode::IsAsciiDigit(character)) return result;

		hasDigit = true;
		const int digit = character - '0';

		length *= 10;
		length += digit;
	}

	result.isValid = hasDigit;
	result.coordinateLength = static_cast<int16_t>(length * sign);
	return result;
}


void System::Receiver::Interpret::Coordinate::ConfirmCommand(
	const SystemContext& context,
	Component::Receiver& receiver
)
{
	constexpr char RESPONSE[] = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	const Sound& response = context.store.CreateSoundHandle(RESPONSE);
	Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Medium);

	receiver.message.clear();
}


bool System::Receiver::Interpret::Coordinate::TryParseSuffix(const char suffix, CoordResult& result)
{
	switch (suffix)
	{
	case 'X': result.axis = CoordResult::Horizontal;
		return true;
	case 'Y': result.axis = CoordResult::Vertical;
		return true;
	default: return false;
	}
}
