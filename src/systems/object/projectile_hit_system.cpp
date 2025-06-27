#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/comms/radar_tags.h"
#include "components/objects/health_component.h"
#include "components/objects/outside/blip_component.h"
#include "components/objects/outside/projectile_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "systems/object/projectile_hit_system.h"
#include "utility/vector2.h"
#include <cstdint>
#include <vector>


void ProjectileHitSystem::Update(
	entt::registry& registry, float deltaTime
)
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
		SoundSystem::PlayEmitter(emitter);
	}

	CheckForHits(registry, hitPositions);
}


void ProjectileHitSystem::CheckForHits(
	entt::registry& registry, 
	std::vector<Nc::Vector2f>& hitPositions
)
{
	auto view = registry.view<Component::Blip, Component::Transform, Component::Health>();
	for (auto [entity, blip, transform, health] : view.each())
	{
		for (Nc::Vector2f hitPosition : hitPositions)
		{
			constexpr float BLAST_RADIUS = 5.0f;
			constexpr int16_t DAMAGE = 10;

			Nc::Vector2f difference = hitPosition - transform.position;
			if (difference.GetSqrDistance() > BLAST_RADIUS * BLAST_RADIUS) continue;
			health.health -= DAMAGE;
		}
	}
};