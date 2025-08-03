#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/comms/radar_tags.hpp"
#include "components/objects/health_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "components/objects/outside/projectile_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.hpp"
#include "systems/object/outside/projectile_hit_system.hpp"
#include "utility/vector2.hpp"
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

	if (hitPositions.empty()) return;

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
			constexpr float BLAST_RADIUS = 12.0f;
			constexpr int16_t DAMAGE = 10;

			Nc::Vector2f difference = hitPosition - transform.position;
			if (difference.GetSqrDistance() > BLAST_RADIUS * BLAST_RADIUS) continue;
			health.health -= DAMAGE;
		}
	}
};