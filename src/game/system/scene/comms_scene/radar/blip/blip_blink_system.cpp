#include "game/system/scene/comms_scene/radar/blip/blip_blink_system.hpp"

#include <cmath>

#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"

#ifdef DEBUG_BUILD
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#endif


void System::Blip::Blink::Update(entt::registry& registry)
{
	const auto view = registry.view<const Tag::Radar::Path, const Component::Transform>();
	for (auto [entity, transform] : view.each())
		UpdateBlips(registry, transform);
}


void System::Blip::Blink::UpdateBlips(entt::registry& registry, const Component::Transform& pathTransform)
{
	const auto view = registry.view<const Component::Blip, const Component::Transform, Component::TweenCollection>();
	for (auto [entity, blip, transform, collection] : view.each())
	{
		if (!blip.isActive) continue;

		Nc::Tween& fadeInTween = collection.tweens.at(Component::Blip::BlipFadeIn);
		if (!BlipShouldAppear(registry, transform, pathTransform, fadeInTween)) continue;
		Nc::Tween::Replay(fadeInTween);
	}
}


bool System::Blip::Blink::BlipShouldAppear(
	entt::registry& registry,
	const Component::Transform& blip,
	const Component::Transform& path,
	const Nc::Tween& tween
)
{
	constexpr Nc::Vector2f WORLD_Y_RANGE = Nc::Vector2f(WORLD_BOUNDS.min.y, WORLD_BOUNDS.max.y);
	constexpr Nc::Vector2f RADAR_Y_RANGE = Nc::Vector2f(RADAR_BOUNDS.min.y, RADAR_BOUNDS.max.y);
	constexpr float DIFFERENCE_THRESHOLD = 0.8f;

	const float pathHeight = path.position.y + path.offset.y;
	const float blipHeight = Nc::Math::Remap(WORLD_Y_RANGE, RADAR_Y_RANGE, blip.position.y);
	const float distance = std::abs(pathHeight - blipHeight);

#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(registry);
	const auto& readouts = registry.get<Component::Debug::RuntimeReadouts>(debugEntity);

	const float threshold = DIFFERENCE_THRESHOLD * readouts.timeScale;
	if (distance > threshold) return false;
#else
	if (distance < DIFFERENCE_THRESHOLD) return false;
#endif
	if (tween.isPlaying) return false;
	return true;
}
