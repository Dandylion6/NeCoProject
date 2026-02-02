#include "game/system/core/tween_system.hpp"

#include "core/data/tween.hpp"
#include "core/math/interpolation.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Tween::Update(const SystemContext& context)
{
	const auto view = context.registry.view<Component::TweenCollection>();
	for (auto [entity, tweenCollection] : view.each())
	{
		// World tweens will be paused with game.
		if (context.registry.all_of<Component::Transform>(entity) && context.game.isPaused) continue;
		UpdateTweenCollection(tweenCollection, context.deltaTime);
	}
}


void System::Tween::UpdateTweenCollection(Component::TweenCollection& collection, const float deltaTime)
{
	for (Nc::Tween& tween : collection.tweens)
	{
		tween.justCompleted = false;
		if (tween.state != Nc::Tween::Playing) continue;
		if (tween.value == nullptr) continue;

		const float totalTime = tween.duration + tween.delayComplete;
		if (tween.elapsed >= totalTime)
		{
			TweenEnded(tween);
			continue;
		}

		tween.elapsed += deltaTime;
		const float cappedTime = std::fminf(tween.elapsed, tween.duration);

		const float normalizedTime = cappedTime / tween.duration;
		const float easeTime = Nc::Tween::GetEasing(tween.easing, normalizedTime);

		*tween.value = Nc::Math::Lerp(tween.start, tween.end, easeTime);
	}
}


void System::Tween::TweenEnded(Nc::Tween& tween)
{
	if (tween.state == Nc::Tween::Playing)
	{
		tween.state = Nc::Tween::Completed;
		tween.justCompleted = true;
	}
	tween.elapsed = 0.0f;
}
