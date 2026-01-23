#include "game/system/scene/outside_scene/receiver/interpret_aiming_system.hpp"

#include <string>

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_coordinate_system.hpp"


void System::Receiver::Interpret::Aiming::HandleMessage(const SystemContext& context, Component::Receiver& receiver, const std::string& message)
{
	if (message == COMMAND)
	{
		receiver.currentContext = AimingArtillery;
		return ConfirmCommand(context, receiver);
	}

	const CoordResult result = Coordinate::MessageAsCoord(message);
	if (!result.isValid) return;

	SetArtilleryTarget(context.registry, result);
	Coordinate::ConfirmCommand(context, receiver);
}


void System::Receiver::Interpret::Aiming::ConfirmCommand(const SystemContext& context, Component::Receiver& receiver)
{
	constexpr char AIM_RESPONSE[] = "assets/audio/voicelines/receiver/commands/aim_request.wav";

	const Sound& response = context.store.CreateSoundHandle(AIM_RESPONSE);
	Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Medium);
	receiver.message.clear();
}


void System::Receiver::Interpret::Aiming::SetArtilleryTarget(
	entt::registry& registry, const CoordResult result
)
{
	constexpr float DELAY_SECONDS = 1.25f;

	const entt::entity entity = entt::get_single<Component::Artillery>(registry);
	auto& artillery = registry.get<Component::Artillery>(entity);

	// Can't fire artillery if aiming.
	artillery.aimStartupSeconds = DELAY_SECONDS;
	artillery.isReadyToFire = false;

	switch (result.axis)
	{
	case CoordResult::Invalid: break;
	case CoordResult::Horizontal:
		artillery.targetPosition.x = result.coordinateLength;
		break;
	case CoordResult::Vertical:
		artillery.targetPosition.y = result.coordinateLength;
		break;
	}
};