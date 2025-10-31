#pragma once
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "entt/entity/fwd.hpp"


class BlipGlitchSystem
{
public:
	static void Update(entt::registry& registry, float time, float deltaTime);
	
	static void JumbleBlip(entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability);
	static void GlitchBlipText(entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability);
	static void TriggerBlipFailure(entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability);

private:
	static void UpdateBlipTextStable(const Component::Transform& transform, const Component::Blip& blip, Component::Text& text);
	static void UpdateBlipTextJumble(entt::registry& registry, entt::entity entity, float time);
	static void UpdateBlipTextError(entt::registry& registry, entt::entity entity, float time);
	static void UpdateBlipFailure(entt::registry& registry, entt::entity entity, float time);

	/// <summary>
	/// Generates and returns a duration value for a glitch effect.
	/// </summary>
	/// <param name="stability">The current stability level of the radar machine, from 0 to 100.</param>
	/// <returns>A duration in seconds.</returns>
	static float GenerateGlitchDuration(float stability);
	static Component::BlipState::JumbledCoordindate GenerateRandomJumble(float stability);

};