#include "game/component/core/tween_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_death_system.hpp"
#include "core/data/tween.hpp"


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