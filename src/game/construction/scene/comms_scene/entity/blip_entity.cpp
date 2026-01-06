#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/construction/scene/comms_scene/entity/blip_entity.hpp"
#include "game/state/scene.hpp"
#include "game/utility/color_palette.hpp"
#include "raylib.h"
#include <cstdint>
#include <utility>


entt::entity Entity::Blip::Create(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Nc::Vector2f position, 
	int16_t health
) noexcept
{
	entt::entity entity = registry.create();

	registry.emplace<Component::Blip>(entity);
	registry.emplace<Component::Health>(entity, health);

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_blip.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	
	registry.emplace<Component::Transform>(entity, RadarScene, position, size, size * 0.5f);
	registry.emplace<Component::Text>(entity, "( , )", Palette::RADAR_COLOR, Nc::Font::WDXL, Nc::Font::Size::Tiny);

	Component::Sprite& sprite = registry.emplace<Component::Sprite>(entity, std::move(texture));
	Component::TweenCollection& tweens = registry.emplace<Component::TweenCollection>(entity);

	sprite.alpha = 0.0f;
	constexpr float FADE_IN_TIME = 0.3f, FADE_OUT_TIME = 6.5f;
	constexpr float FADE_OUT_DELAY = 1.8f;

	Tween& fadeInTween = tweens.tweens.at(Component::Blip::BlipFadeIn);
	Tween& fadeOutTween = tweens.tweens.at(Component::Blip::BlipFadeOut);

	fadeInTween.value = &sprite.alpha;
	fadeInTween.start = sprite.alpha;
	fadeInTween.end = 1.0f;
	fadeInTween.duration = FADE_IN_TIME;
	fadeInTween.easing = CubicOut;
	fadeInTween.delayComplete = FADE_OUT_DELAY;
	fadeInTween.onComplete = [&fadeOutTween]() { Tween::Replay(fadeOutTween); };

	fadeOutTween.value = &sprite.alpha;
	fadeOutTween.start = 1.0f;
	fadeOutTween.end = 0.0f;
	fadeOutTween.duration = FADE_OUT_TIME;
	fadeOutTween.easing = QuadOut;

	return entity;
}