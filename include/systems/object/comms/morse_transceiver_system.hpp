#pragma once
#include "components/objects/comms/morse_transceiver_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/morse_code.hpp"
#include <cstdint>
struct MorseSettings;


class MorseTransceiverSystem
{
public:
	static void Update(
		entt::registry& registry, 
		Scene currentScene, 
		MorseSettings settings,
		float deltaTime
	);

private:
	static void InputChanged(
		Component::MorseTransceiver& transceiver,
		MorseSettings settings
	);

	static void TryEndCharacter(
		entt::registry& registry, 
		Component::MorseTransceiver& transceiver,
		MorseSettings settings
	);

	static void TransmitCharacter(entt::registry& registry, char character);

	static void RecordPulse(
		Component::MorseTransceiver& transceiver, MorseSettings settings
	);

	static void ClearTransceiver(Component::MorseTransceiver& transceiver);
	static MorseCode::Pulse GetPulseType(
		float intervalSeconds, MorseSettings settings
	);
	
	static char PulsesToChar(
		const Component::MorseTransceiver::PulseArray& pulses, 
		uint8_t pulseCount
	);

};