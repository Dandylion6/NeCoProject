#pragma once
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "game/utility/morse_code.hpp"
#include <cstdint>
struct AnomalyState;
struct GameState;
struct MorseSettings;


class MorseTransceiverSystem
{
public:
	static void Update(
		entt::registry& registry, 
		GameState& gameState, 
		MorseSettings settings,
		float deltaTime
	);

private:
	static void InputChanged(
		Component::Morse::Transceiver& transceiver, MorseSettings settings
	);

	static void TryEndCharacter(
		entt::registry& registry, AnomalyState& anomalyState, Component::Morse::Transceiver& transceiver, MorseSettings settings
	);

	static void TransmitCharacter(entt::registry& registry, AnomalyState& anomalyState, char character);

	static void RecordPulse(
		Component::Morse::Transceiver& transceiver, MorseSettings settings
	);

	static void ClearTransceiver(Component::Morse::Transceiver& transceiver);
	static MorseCode::Pulse GetPulseType(
		float intervalSeconds, MorseSettings settings
	);
	
	static char PulsesToChar(
		const Component::Morse::Transceiver::PulseArray& pulses, uint8_t pulseCount
	);

};