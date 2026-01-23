#include "game/system/scene/outside_scene/receiver/receiver_code_response_system.hpp"

#include <format>
#include <string>
#include <string_view>

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/utility/morse_code.hpp"


void System::Receiver::CodeResponse::Update(const SystemContext& context) noexcept
{
	const entt::entity entity = entt::get_single<Component::Receiver>(context.registry);
	auto& receiver = context.registry.get<Component::Receiver>(entity);

	switch (receiver.incomingCharacter)
	{
	case MorseCode::NULL_CODE:
	case MorseCode::BACK_CODE:
	case MorseCode::CANCEL_CODE:
		return;

	default:
		ConfirmCodeReceived(context, receiver);
		receiver.incomingCharacter = MorseCode::NULL_CODE;
		break;
	}
}


void System::Receiver::CodeResponse::ConfirmCodeReceived(const SystemContext& context, Component::Receiver& receiver)
{
	constexpr std::string_view FORMAT = "assets/audio/voicelines/receiver/{}_received.wav";

	const std::string filePath = std::format(FORMAT, receiver.incomingCharacter);
	const Sound& response = context.store.CreateSoundHandle(filePath);
	Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Low);
}
