#pragma once
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"


class BlipGlitchSystem
{
public:
	static void Update(entt::registry& registry, float time, float deltaTime);
	
	static void JumbleBlip(entt::registry& registry, entt::entity entity, Component::Blip& blip);
	static void GlitchBlipText(entt::registry& registry, entt::entity entity, Component::Blip& blip);
	static void TriggerBlipFailure(entt::registry& registry, entt::entity entity, Component::Blip& blip);

private:
	static void UpdateBlipTextStable(const Component::Transform& transform, const Component::Blip& blip, Component::Text& text);
	static void UpdateBlipTextJumble(entt::registry& registry, entt::entity entity, float time);
	static void UpdateBlipTextError(entt::registry& registry, entt::entity entity, float time);
	static void UpdateBlipFailure(entt::registry& registry, entt::entity entity, float time);

	static Component::Blip::JumbledCoordindate GenerateRandomJumble();

};