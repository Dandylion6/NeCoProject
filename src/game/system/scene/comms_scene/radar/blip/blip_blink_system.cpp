#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_blink_system.hpp"
#include "core/data/interpolation.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include <cmath>

#ifdef DEBUG_BUILD
#include "game/debug/debug_context.hpp"
#include "game/game.hpp"
#endif // DEBUG_BUILD



void BlipBlinkSystem::Update(entt::registry& registry)
{
	
	auto view = registry.view<const Tag::Radar::Path, const Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		UpdateBlips(registry, transform);
	}
}


void BlipBlinkSystem::UpdateBlips(entt::registry &registry, const Component::Transform& pathTransform)
{
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
	constexpr Nc::Vector2f WORLD_Y_RANGE = Nc::Vector2f(WORLD_BOUNDS.min.y, WORLD_BOUNDS.max.y);
	constexpr Nc::Vector2f RADAR_Y_RANGE = Nc::Vector2f(RADAR_BOUNDS.min.y, RADAR_BOUNDS.max.y);
	constexpr float DIFFERENCE_THRESHOLD = 0.8f;

	float pathHeight = path.position.y + path.offset.y;
	float blipHeight = Math::Remap(WORLD_Y_RANGE, RADAR_Y_RANGE, blip.position.y);
	float distance = std::fabsf(pathHeight - blipHeight);
	
	float threshold = DIFFERENCE_THRESHOLD;
#ifdef DEBUG_BUILD
	threshold *= Game::debugContext.timeScale;
#endif // DEBUG_BUILD

	if (distance > threshold) return false;
	if (tween.isPlaying) return false;
	return true;
}
