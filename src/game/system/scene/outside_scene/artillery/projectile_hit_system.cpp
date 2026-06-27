#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/outside_scene/projectile_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/scene/outside_scene/artillery/projectile_hit_system.hpp"
#include "raylib.h"
#include <cstdint>
#include <vector>

#include "game/component/core/audio/audio_emitter_component.hpp"


void System::Projectile::Hit::Update(const SystemContext& context) noexcept
{
	size_t hitCount = 0u;
	Nc::Vector2f hitPositions[MAX_HITS] = { };

	const auto view = context.registry.view<const Component::Transform, Component::Projectile, Component::AudioEmitter>();
	for (auto [entity, transform, projectile, emitter] : view.each())
	{
		if (!projectile.isActive)
		{
			if (emitter.state == Component::AudioEmitter::Playing)
				context.registry.destroy(entity);
			continue;
		}

		if (projectile.travelSecondsLeft > 0.0f)
		{
			projectile.travelSecondsLeft -= context.deltaTime;
			continue;
		}

		projectile.isActive = false;
		hitPositions[hitCount++] = transform.position;
		Audio::Emitter::PlayEmitter(emitter);
	}

	if (hitCount > 0)
		CheckForHits(context.registry, hitPositions, hitCount);
}


void System::Projectile::Hit::CheckForHits(
	entt::registry& registry,
	Nc::Vector2f hitPositions[MAX_HITS],
	const size_t hitCount
) noexcept
{
	constexpr float BLAST_RADIUS = 12.0f;
	constexpr float SQR_BLAST_RADIUS = BLAST_RADIUS * BLAST_RADIUS;

	const auto view = registry.view<Component::Blip, Component::Transform, Component::Health>();
	for (auto [entity, blip, transform, health] : view.each())
	{
		for (size_t i = 0; i < hitCount; ++i)
		{
			constexpr int16_t DAMAGE = 10;

			const Nc::Vector2f hitPosition = hitPositions[i];
			const float sqrDistance = Nc::Vector::SqrDistanceBetween(hitPosition, transform.position);
			if (sqrDistance <= SQR_BLAST_RADIUS)
				health.health -= DAMAGE;
		}
	}
};
