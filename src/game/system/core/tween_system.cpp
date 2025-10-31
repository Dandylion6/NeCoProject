#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/core/tween_system.hpp"
#include "core/data/interpolation.hpp"
#include "core/data/tween.hpp"
#include <algorithm>


void TweenSystem::Update(entt::registry& registry, GameState& gameState, float deltaTime)
{
	auto view = registry.view<Component::TweenCollection>();
	for (auto [entity, tweenCollection] : view.each())
	{
		if (registry.all_of<Component::Transform>(entity) && gameState.isPaused) continue;
		UpdateTweenCollection(tweenCollection, deltaTime);
	}
}


void TweenSystem::UpdateTweenCollection(Component::TweenCollection& collection, float deltaTime)
{
	for (Tween& tween : collection.tweens)
	{
		if (!tween.isPlaying) continue;
		if (tween.elapsed >= tween.duration + tween.delayComplete)
		{
			TweenEnded(tween);
			continue;
		}

		tween.elapsed += deltaTime;
		float cappedTime = std::min(tween.elapsed, tween.duration);

		float normalizedTime = cappedTime / tween.duration;
		float easeTime = Tween::GetEasing(tween.easing, normalizedTime);

		*tween.value = Math::Lerp(tween.start, tween.end, easeTime);
	}
}


void TweenSystem::TweenEnded(Tween& tween)
{
	tween.isPlaying = false;
	tween.elapsed = 0.0f;
	if (tween.onComplete != nullptr)
	{
		tween.onComplete();
	}
}