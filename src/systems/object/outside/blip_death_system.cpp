#include "components/core/tween_component.h"
#include "components/objects/health_component.h"
#include "components/objects/outside/blip_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/outside/blip_death_system.h"
#include "utility/tween.h"


void BlipDeathSystem::Update(entt::registry& registry)
{
	auto view = registry.view<const Component::Health, Component::Blip, Component::TweenCollection>();
	for (auto [entity, health, blip, collection] : view.each())
	{
		if (health.health > 0) continue;
		blip.isActive = false;
		
		Tween& fadeIn = collection.tweens.at(Component::Blip::BlipFadeIn);
		Tween& fadeOut = collection.tweens.at(Component::Blip::BlipFadeOut);
		if (fadeIn.isPlaying || fadeOut.isPlaying) continue;

		registry.destroy(entity);
	}
}