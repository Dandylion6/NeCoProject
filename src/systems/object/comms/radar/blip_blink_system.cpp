#include "components/core/transform_component.hpp"
#include "components/core/tween_component.hpp"
#include "components/objects/comms/radar_tags.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/blip_blink_system.hpp"
#include "utility/tween.hpp"
#include "utility/vector2.hpp"
#include <cmath>


void BlipBlinkSystem::Update(entt::registry& registry)
{
	entt::entity radarPathEntity = registry.view<const Tag::RadarPath>().front();
	const Component::Transform& pathTransform = registry.get<const Component::Transform>(radarPathEntity);

	auto view = registry.view<const Component::Blip, const Component::Transform, Component::TweenCollection>();
	for (auto [entity, blip, transform, tweens] : view.each())
	{
		if (!blip.isActive) continue;
		
		Tween& fadeInTween = tweens.tweens.at(Component::Blip::BlipFadeIn);
		if (!BlipShouldAppear(transform, pathTransform, fadeInTween)) continue;
		Tween::Replay(fadeInTween);
	}
}


bool BlipBlinkSystem::BlipShouldAppear(
	Component::Transform blip, 
	Component::Transform path,
	Tween& tween
)
{
	float pathHeight = path.position.y + path.offset.y;
	float blipHeight = blip.position.y + blip.offset.y;
	float distance = std::fabsf(pathHeight - blipHeight);
	
	if (distance > 1.0f) return false;
	if (tween.isPlaying) return false;
	return true;
}
