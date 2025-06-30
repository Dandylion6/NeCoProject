#include "cctype"
#include "components/objects/comms/radio_component.h"
#include "components/objects/outside/artillery_component.h"
#include "components/objects/outside/receiver_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.h"
#include "systems/object/outside/receiver/artillery_control_system.h"
#include <cstdint>
#include <string>
#include <utility>
#include "core/render_context.h"


const std::string ArtilleryControlSystem::COMMAND = "AIM";


void ArtilleryControlSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		receiver.currentContext = AimingArtillery;
		return ConfirmAimCommand(registry, resourceStore, receiver);
	}
	HandleMessageAsCoord(registry, resourceStore, receiver, message);
}


void ArtilleryControlSystem::ConfirmAimCommand(
	entt::registry& registry, 
	ResourceStore& resourceStore, 
	Component::Receiver& receiver
)
{
	const std::string AIM_RESPONSE = "assets/audio/voicelines/receiver/commands/aim_request.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(AIM_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void ArtilleryControlSystem::HandleMessageAsCoord(
	entt::registry& registry, 
	ResourceStore& resourceStore, 
	Component::Receiver& receiver, 
	const std::string& message
)
{
	CoordResult result = InterpretMessageAsCoord(message);
	if (!result.isValid)
	{
		if (result.withinRegion) return;
		
		receiver.message.clear();
	}

	SetArtilleryTarget(registry, result);

	const std::string COORDINATE_RESPONSE = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(COORDINATE_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


ArtilleryControlSystem::CoordResult ArtilleryControlSystem::InterpretMessageAsCoord(
	const std::string& message
)
{
	CoordResult result { };
	for (uint32_t i = 0u; i < message.length(); ++i)
	{
		const char character = message.at(i);

		bool isLastCharacter = i == (message.length() - 1);
		if (isLastCharacter)
		{
			if (character == 'X') result.axis = CoordResult::Horizontal;
			else if (character == 'Y') result.axis = CoordResult::Vertical;

		} else
		{
			if (!std::isdigit(character)) return result;

			int16_t digit = character - '0';
			result.coordinateLength *= 10;
			result.coordinateLength += digit;
		}
	}

	bool validAxis = result.axis != CoordResult::Invalid;
	bool hasLength = result.coordinateLength > 0;

	float regionLength = 0.0f;
	switch (result.axis)
	{
	case CoordResult::Horizontal:
		regionLength = RenderContext::RADAR_SIZE.x;
		break;
	case CoordResult::Vertical:
		regionLength = RenderContext::RADAR_SIZE.y;
		break;
	default:
		break;
	}

	result.withinRegion = result.coordinateLength <= regionLength;
	result.isValid = validAxis && hasLength && result.withinRegion;
	return result;
}


void ArtilleryControlSystem::SetArtilleryTarget(
	entt::registry& registry, CoordResult result
)
{
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		constexpr float DELAY = 1.25f;
		artillery.aimStartupDelay = DELAY;
		artillery.isReadyToFire = false;

		switch (result.axis)
		{
		case CoordResult::Invalid:
			break;
		case CoordResult::Horizontal:
			artillery.targetPosition.x = result.coordinateLength;
			break;
		case CoordResult::Vertical:
			artillery.targetPosition.y = result.coordinateLength;
			break;
		}
	}
};