#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/outside_scene/projectile_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/system/core/audio/sound_emitter_system.hpp"
#include "game/system/scene/outside_scene/artillery/projectile_hit_system.hpp"
#include "raylib.h"
#include <cstdint>
#include <vector>


void ProjectileHitSystem::Update(entt::registry& registry, float deltaTime)
{
	std::vector<Nc::Vector2f> hitPositions { };

	auto view = registry.view<Component::Projectile, Component::SoundEmitter>();
	for (auto [entity, projectile, emitter] : view.each())
	{
		if (!projectile.isActive)
		{
			if (!IsSoundPlaying(emitter.sound)) registry.destroy(entity);
			continue;
		}

		if (projectile.travelTimeLeft > 0.0f)
		{
			projectile.travelTimeLeft -= deltaTime;
			continue;
		}

		projectile.isActive = false;
		hitPositions.push_back(projectile.hitPosition);
		SoundEmitterSystem::PlayEmitter(emitter);
	}

	if (hitPositions.empty()) return;

	CheckForHits(registry, hitPositions);
}


void ProjectileHitSystem::CheckForHits(entt::registry& registry, std::vector<Nc::Vector2f>& hitPositions)
{
	constexpr float BLAST_RADIUS = 12.0f;
	constexpr float SQR_BLAST_RADIUS = BLAST_RADIUS * BLAST_RADIUS;
	constexpr int16_t DAMAGE = 10;

	auto view = registry.view<Component::Blip, Component::Transform, Component::Health>();
	for (auto [entity, blip, transform, health] : view.each())
	{
		for (Nc::Vector2f hitPosition : hitPositions)
		{
			float sqrDistance = Nc::Vector::SqrDistanceOf(hitPosition, transform.position);
			if (sqrDistance > BLAST_RADIUS * BLAST_RADIUS) continue;
			health.health -= DAMAGE;
		}
	}
};