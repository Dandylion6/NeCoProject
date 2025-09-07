#include "components/core/transform_component.hpp"
#include "components/core/tween_component.hpp"
#include "components/objects/comms/radar_tags.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/blip_blink_system.hpp"
#include "utility/interpolation.hpp"
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
	constexpr Nc::Vector2f WORLD_Y_RANGE = Nc::Vector2f(GameState::WORLD_BOUNDS.min.y, GameState::WORLD_BOUNDS.max.y);
	constexpr Nc::Vector2f RADAR_Y_RANGE = Nc::Vector2f(RenderContext::RADAR_BOUNDS.min.y, RenderContext::RADAR_BOUNDS.max.y);

	float pathHeight = path.position.y + path.offset.y;
	float blipHeight = Math::Remap(WORLD_Y_RANGE, RADAR_Y_RANGE, blip.position.y) - blip.offset.y;
	float distance = std::fabsf(pathHeight - blipHeight);
	
	if (distance > 0.1f) return false;
	if (tween.isPlaying) return false;
	return true;
}
