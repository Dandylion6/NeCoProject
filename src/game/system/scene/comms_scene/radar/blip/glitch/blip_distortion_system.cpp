#include "game/system/scene/comms_scene/radar/blip/glitch/blip_distortion_system.hpp"

#include <format>
#include <string>

#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Blip::Jumble::Initialize(
	entt::registry& registry,
	const entt::entity entity,
	Component::Blip& blip,
	float stability
)
{
	blip.state = Component::Blip::CoordinateJumble;
	Component::Glitch::Distortion& jumble = registry.emplace<Component::Glitch::Distortion>(entity, stability);

	auto& randomService = registry.ctx().get<Nc::Random>();
	jumble = GenerateRandomJumble(randomService, stability);
}


void System::Blip::Jumble::Update(const SystemContext& context)
{
	auto& randomService = context.registry.ctx().get<Nc::Random>();

	const auto view = context.registry.view<Component::Blip, Component::Transform, Component::Glitch::Distortion,
	                                        Component::Text>();
	for (auto [entity, blip, transform, jumble, text] : view.each())
	{
		jumble.jumbleSecondsLeft -= context.deltaTime;
		blip.remainingGlitchSeconds -= context.deltaTime;

		if (blip.remainingGlitchSeconds <= 0.0f)
		{
			blip.state = Component::Blip::Stable;
			context.registry.remove<Component::Glitch::Distortion>(entity);
			continue;
		}

		const Nc::Vector2i displayedPosition = GetDisplayPosition(transform, jumble);
		text.text = "{" + std::to_string(displayedPosition.x) + " , " + std::to_string(displayedPosition.y) + "}";

		if (jumble.jumbleSecondsLeft <= 0.0f)
			jumble = GenerateRandomJumble(randomService, jumble.stability);
	}
}


Nc::Vector2i System::Blip::Jumble::GetDisplayPosition(
	const Component::Transform& transform,
	const Component::Glitch::Distortion jumble
)
{
	const Nc::Vector2i pixelPosition = Nc::Vector2i(transform.position);
	Nc::Vector2i displayedPosition = pixelPosition;

	if (jumble.flippedAxis)
	{
		displayedPosition.x = pixelPosition.y;
		displayedPosition.y = pixelPosition.x;
	}

	displayedPosition.y = jumble.duplicateFirstAxis ? displayedPosition.x : displayedPosition.y;
	displayedPosition.x *= jumble.flippedSignX ? -1 : 1;

	return displayedPosition;
}


Component::Glitch::Distortion System::Blip::Jumble::GenerateRandomJumble(
	Nc::Random& randomService,
	const float stability
)
{
	constexpr Nc::Vector2f INTERVAL_LOW_STABILITY = Component::Glitch::Distortion::JUMBLE_INTERVAL_LOW_RANGE;
	constexpr Nc::Vector2f INTERVAL_HIGH_STABILITY = Component::Glitch::Distortion::JUMBLE_INTERVAL_HIGH_RANGE;
	constexpr Nc::Vector2f STABILITY_RANGE = Nc::Vector2f(
		Component::Radar::STABLE_LEVEL,
		Component::Radar::HEALTHY_LEVEL
	);
	constexpr Nc::Vector2f DEGRADATION_SCALE_RANGE = Nc::Vector2f(0.0f, 1.0f);

	auto jumble = Component::Glitch::Distortion(stability);

	jumble.duplicateFirstAxis = randomService.RangeInt(0, 10) <= 2;
	jumble.flippedAxis = randomService.RangeInt(0, 10) <= 6;
	jumble.flippedSignX = randomService.RangeInt(0, 10) <= 4;
	jumble.flippedSignY = randomService.RangeInt(0, 10) <= 4;

	float degradationScale = Nc::Math::Remap(
		STABILITY_RANGE,
		DEGRADATION_SCALE_RANGE,
		std::fmaxf(jumble.stability, Component::Radar::HEALTHY_LEVEL)
	);
	degradationScale = Nc::Math::SineInOut(degradationScale);
	const Nc::Vector2f range = Nc::Vector::Lerp(INTERVAL_HIGH_STABILITY, INTERVAL_LOW_STABILITY, degradationScale);

	float randomValue = randomService.RangeFloat(0.0f, 1.0f);
	randomValue = Nc::Math::QuadIn(randomValue);
	jumble.jumbleSecondsLeft = Nc::Math::Lerp(range.x, range.y, randomValue);

	return jumble;
}
