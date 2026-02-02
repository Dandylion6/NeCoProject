#include "game/system/scene/comms_scene/morse_code/morse_transceiver_system.hpp"

#include <cmath>
#include <cstdint>
#include <string>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/state/settings.hpp"
#include "game/utility/morse_code.hpp"

#ifdef DEBUG_BUILD
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#endif


void System::Morse::Transceiver::Update(const SystemContext& context, AnomalyState& anomaly, const Settings::Morse settings)
{
	const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(context.registry);
	const auto& transform = context.registry.get<Component::Transform>(entity);
	auto& transceiver = context.registry.get<Component::Morse::Transceiver>(entity);

	if (context.game.currentScene != transform.boundScene) return;

	const bool inputKeyPressed = IsKeyDown(Component::Morse::Transceiver::INPUT_KEY);

	if (inputKeyPressed != transceiver.isInputActive) InputChanged(transceiver, settings);
	else if (!inputKeyPressed) TryEndCharacter(context.registry, anomaly, transceiver, settings);

	transceiver.isInputActive = inputKeyPressed;

	const float increasedInterval = transceiver.intervalSeconds + context.deltaTime;
	const float exitTime = MorseCode::ExitTime(settings.dotTime);
	transceiver.intervalSeconds = std::fminf(increasedInterval, exitTime);

#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(context.registry);
	auto& readout = context.registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	readout.pulse = transceiver.isInputActive ?
		                GetPulseType(transceiver.intervalSeconds, settings) :
		                MorseCode::Invalid;
#endif // DEBUG_BUILD
}


void System::Morse::Transceiver::InputChanged(
	Component::Morse::Transceiver& transceiver,
	const Settings::Morse settings
)
{
	if (IsKeyPressed(Component::Morse::Transceiver::INPUT_KEY))
	{
		transceiver.intervalSeconds = 0.0f;
	} else
	{
		RecordPulse(transceiver, settings); // Input just stopped
	}
}


void System::Morse::Transceiver::TryEndCharacter(
	entt::registry& registry,
	AnomalyState& anomalyState,
	Component::Morse::Transceiver& transceiver,
	const Settings::Morse settings
)
{
	const float longestTime = MorseCode::ExitTime(settings.dotTime);
	const bool shouldEndCharacter = transceiver.intervalSeconds >= longestTime;

	if (!shouldEndCharacter) return;
	if (transceiver.pulseCount == 0u) return;

	const int8_t character = PulsesToChar(transceiver.pulses, transceiver.pulseCount);

	TransmitCharacter(registry, anomalyState, character);
	ClearTransceiver(transceiver);
}


void System::Morse::Transceiver::TransmitCharacter(
	entt::registry& registry,
	AnomalyState& anomalyState,
	const char character
)
{
	constexpr float ATTRACTION_INCREASE = 0.42f;

	const entt::entity entity = entt::get_single<Component::Receiver>(registry);
	auto& receiver = registry.get<Component::Receiver>(entity);

	// Transmission increases attraction level
	anomalyState.attractionPercentage += ATTRACTION_INCREASE;
	receiver.incomingCharacter = character;
}


void System::Morse::Transceiver::RecordPulse(Component::Morse::Transceiver& transceiver, const Settings::Morse settings)
{
	if (transceiver.pulseCount >= Component::Morse::Transceiver::MAX_PULSES) return;

	transceiver.pulses[transceiver.pulseCount] = GetPulseType(transceiver.intervalSeconds, settings);
	++transceiver.pulseCount;
	transceiver.intervalSeconds = 0.0f;
}


void System::Morse::Transceiver::ClearTransceiver(Component::Morse::Transceiver& transceiver)
{
	transceiver.intervalSeconds = 0.0f;
	transceiver.pulses.fill(MorseCode::Invalid);
	transceiver.pulseCount = 0u;
}


MorseCode::Pulse System::Morse::Transceiver::GetPulseType(const float intervalSeconds, const Settings::Morse settings)
{
	const float errorMargin = MorseCode::ErrorMargin(settings.dotTime);

	Nc::Vector2f margins = Nc::Vector2f::Zero();
	margins.x = settings.dotTime - errorMargin;
	margins.y = settings.dotTime + errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Short;

	const float dashTime = MorseCode::DashTime(settings.dotTime);

	margins.x = dashTime - errorMargin;
	margins.y = dashTime + errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Long;

	return MorseCode::Invalid;
}


char System::Morse::Transceiver::PulsesToChar(
	const Component::Morse::Transceiver::PulseArray& pulses,
	const uint8_t pulseCount
)
{
	std::string codeString{ };
	for (uint8_t i = 0u; i < pulseCount; ++i)
	{
		switch (pulses.at(i))
		{
		case MorseCode::Invalid: return MorseCode::NULL_CODE;
		case MorseCode::Short:
			codeString += '.';
			break;
		case MorseCode::Long:
			codeString += '-';
			break;
		default: return MorseCode::NULL_CODE;
		}
	}
	return MorseCode::GetChar(codeString);
}
