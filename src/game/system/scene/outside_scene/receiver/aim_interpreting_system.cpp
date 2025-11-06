#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/scene/comms_scene/radio/radio_sound_system.hpp"
#include "game/system/scene/outside_scene/receiver/aim_interpreting_system.hpp"
#include <string>
#include <utility>
#include "game/system/scene/outside_scene/receiver/coordinate_interpreting_system.hpp"


const std::string AimInterpretingSystem::COMMAND = "AIM";


void AimInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	Nc::ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		receiver.currentContext = AimingArtillery;
		return ConfirmAimCommand(registry, resourceStore, receiver);
	}

	CoordResult result = CoordinateInterpretingSystem::InterpretMessageAsCoord(registry, resourceStore, receiver, message);
	if (!result.isValid) return;

	SetArtilleryTarget(registry, result);

	CoordinateInterpretingSystem::ConfirmCoordinateCommand(registry, resourceStore, receiver);
}


void AimInterpretingSystem::ConfirmAimCommand(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Component::Receiver& receiver
)
{
	const std::string AIM_RESPONSE = "assets/audio/voicelines/receiver/commands/aim_request.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(AIM_RESPONSE));
	RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void AimInterpretingSystem::SetArtilleryTarget(
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