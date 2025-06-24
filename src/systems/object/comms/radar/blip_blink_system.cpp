#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/objects/comms/radar_tags.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/blip_blink_system.h"
#include "utility/tween.h"
#include "utility/vector2.h"
#include <cmath>


void BlipBlinkSystem::Update(entt::registry& registry)
{
	entt::entity radarPathEntity = registry.view<const Tag::RadarPath>().front();
	const Component::Transform& pathTransform = registry.get<const Component::Transform>(radarPathEntity);

	auto blipView = registry.view<const Tag::Blip, const Component::Transform, Component::TweenCollection>();
	for (auto [entity, transform, tweens] : blipView.each())
	{
		Tween& fadeInTween = tweens.tweens.at(Tag::Blip::BlipFadeIn);
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
