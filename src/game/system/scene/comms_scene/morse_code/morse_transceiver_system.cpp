#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/state/settings.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/scene/comms_scene/morse_code/morse_transceiver_system.hpp"
#include "game/utility/morse_code.hpp"
#include "core/data/vector2.hpp"
#include <cmath>
#include <cstdint>
#include <string>

#ifdef DEBUG_BUILD
#include "game/debug/debug_context.hpp"
#include "game/game.hpp"
#endif // DEBUG_BUILD


void MorseTransceiverSystem::Update(
	entt::registry& registry, GameState& gameState, MorseSettings settings, float deltaTime
)
{
	auto view = registry.view<Component::Transform, Component::Morse::Transceiver>();
	for (auto [entity, transform, transceiver] : view.each())
	{
		if (gameState.currentScene != transform.boundScene) continue;

		bool inputKeyPressed = IsKeyDown(Component::Morse::Transceiver::INPUT_KEY);
		bool inputStateChanged = inputKeyPressed != transceiver.isInputActive;

		if (inputStateChanged) InputChanged(transceiver, settings);
		else if (!inputKeyPressed) TryEndCharacter(registry, gameState.anomalyState, transceiver, settings);

		transceiver.isInputActive = inputKeyPressed;
		
		float increasedInverval = transceiver.intervalSeconds + deltaTime;
		transceiver.intervalSeconds = std::fminf(increasedInverval, settings.exitTime);

#ifdef DEBUG_BUILD
		if (!transceiver.isInputActive)
		{
			Game::debugContext.pulse = MorseCode::Invalid;
			continue;
		}
		Game::debugContext.pulse = GetPulseType(transceiver.intervalSeconds, settings);
#endif // DEBUG_BUILD
	}
}


void MorseTransceiverSystem::InputChanged(
	Component::Morse::Transceiver& transceiver, MorseSettings settings
)
{
	bool inputJustStarted = IsKeyPressed(Component::Morse::Transceiver::INPUT_KEY);
	if (inputJustStarted)
	{
		transceiver.intervalSeconds = 0.0f;
	} else RecordPulse(transceiver, settings); // Input just stopped
}


void MorseTransceiverSystem::TryEndCharacter(
	entt::registry& registry, AnomalyState& anomalyState, Component::Morse::Transceiver& transceiver, MorseSettings settings
)
{
	float longestTime = settings.dashTime + settings.errorMargin;
	bool shouldEndCharacter = transceiver.intervalSeconds > longestTime;
	
	if (!shouldEndCharacter) return;
	if (transceiver.pulseCount == 0u) return;

	int8_t character = PulsesToChar(transceiver.pulses, transceiver.pulseCount);

	TransmitCharacter(registry, anomalyState, character);
	ClearTransceiver(transceiver);
}


void MorseTransceiverSystem::TransmitCharacter(
	entt::registry& registry, AnomalyState& anomalyState, char character
)
{
	constexpr float ATTRACTION_INCREASE = 0.42f;

	auto receiverView = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : receiverView.each())
	{
		// Transmission inncreases attraction level
		anomalyState.attractionPercentage += ATTRACTION_INCREASE;
		receiver.incomingCharacter = character;
	}
}


void MorseTransceiverSystem::RecordPulse(
	Component::Morse::Transceiver& transceiver, MorseSettings settings
)
{
	if (transceiver.pulseCount >= Component::Morse::Transceiver::MAX_PULSES) return;
	
	transceiver.pulses[transceiver.pulseCount] = GetPulseType(transceiver.intervalSeconds, settings);
	++transceiver.pulseCount;
	transceiver.intervalSeconds = 0.0f;
}


void MorseTransceiverSystem::ClearTransceiver(Component::Morse::Transceiver& transceiver)
{
	transceiver.intervalSeconds = 0.0f;
	transceiver.pulses.fill(MorseCode::Invalid);
	transceiver.pulseCount = 0u;
}


MorseCode::Pulse MorseTransceiverSystem::GetPulseType(float intervalSeconds, MorseSettings settings)
{
	Nc::Vector2f margins = Nc::Vector2f::Zero();
	margins.x = settings.dotTime - settings.errorMargin;
	margins.y = settings.dotTime + settings.errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Short;
	
	margins.x = settings.dashTime - settings.errorMargin;
	margins.y = settings.dashTime + settings.errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Long;

	return MorseCode::Invalid;
}


char MorseTransceiverSystem::PulsesToChar(
	const Component::Morse::Transceiver::PulseArray& pulses, uint8_t pulseCount
)
{
	std::string codeString { };
	for (uint8_t i = 0u; i < pulseCount; ++i)
	{
		MorseCode::Pulse pulse = pulses.at(i);
		switch (pulse)
		{
		case MorseCode::Invalid: return MorseCode::NULL_CODE;
		case MorseCode::Short: codeString += '.'; break;
		case MorseCode::Long: codeString += '-'; break;
		default: return MorseCode::NULL_CODE;
		}
	}
	return MorseCode::GetChar(codeString);
}
