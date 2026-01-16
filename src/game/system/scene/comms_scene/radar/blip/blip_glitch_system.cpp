#include "game/system/scene/comms_scene/radar/blip/blip_glitch_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Blip::Glitch::Update(const SystemContext& context)
{
	const auto view = context.registry.view<const Component::Transform, Component::Blip, Component::Text>();
	for (auto [entity, transform, blip, text] : view.each())
	{
		if (blip.state != Component::Blip::Stable) continue;

		const Nc::Vector2i displayedPosition = Nc::Vector2i(transform.position);
		text.text = "{" + std::to_string(displayedPosition.x) + " , " + std::to_string(displayedPosition.y) + "}";
    }
}


float System::Blip::Glitch::GenerateGlitchDuration(entt::registry& registry, const float stability)
{
	/**
	 * @brief The time range when radar stability is just below unstable threshold.
	 */
	constexpr Nc::Vector2f BASE_GLITCH_TIME_RANGE = Nc::Vector2f(1.6f, 6.4f);

	/**
	 * @brief The time range when radar stability is at 0.
	 */
	constexpr Nc::Vector2f MAX_GLITCH_TIME_RANGE = Nc::Vector2f(58.0f, 96.0f);
	constexpr float STABLE_FACTOR = 1.0f / Component::Radar::STABLE_LEVEL;

	auto& randomService = registry.ctx().get<Nc::Random>();

	float degradationScale = (Component::Radar::STABLE_LEVEL - stability) * STABLE_FACTOR;
	degradationScale = Nc::Math::SineIn(degradationScale);
	const float glitchTimeMin = Nc::Math::Lerp(BASE_GLITCH_TIME_RANGE.x, MAX_GLITCH_TIME_RANGE.x, degradationScale);
	const float glitchTimeMax = Nc::Math::Lerp(BASE_GLITCH_TIME_RANGE.y, MAX_GLITCH_TIME_RANGE.y, degradationScale);

	return randomService.RangeFloat(glitchTimeMin, glitchTimeMax);
}