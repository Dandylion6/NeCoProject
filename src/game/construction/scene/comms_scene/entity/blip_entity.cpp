#include "game/construction/scene/comms_scene/entity/blip_entity.hpp"

#include "raylib.h"
#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"
#include "game/utility/color_palette.hpp"


entt::entity Entity::Blip::Create(const SceneContext& context, Nc::Vector2f position, int16_t health) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/radar_blip.png";
	constexpr float FADE_IN_TIME = 0.3f, FADE_OUT_TIME = 6.5f;
	constexpr float FADE_OUT_DELAY = 1.8f;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Blip>(entity);
	context.registry.emplace<Component::Health>(entity, health);

	Texture2D texture = context.store.GetTexture(FILE_PATH);
	auto& sprite = context.registry.emplace<Component::Sprite>(entity, texture);
	sprite.alpha = 0.0f;

	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	context.registry.emplace<Component::Transform>(entity, RadarScene, position, size, size * 0.5f);

	context.registry.emplace<Component::Text>(
		entity,
		"( , )",
		Palette::RADAR_COLOR,
		Nc::Font::WDXL,
		Nc::Font::Size::Tiny
	);

	auto& collection = context.registry.emplace<Component::TweenCollection>(entity);

	Nc::Tween& fadeOutTween = collection.tweens.at(Component::Blip::BlipFadeOut);
	Nc::Tween::Build(fadeOutTween, &sprite.alpha, 1.0f, 0.0f, FADE_OUT_TIME, QuadOut);

	Nc::Tween& fadeInTween = collection.tweens.at(Component::Blip::BlipFadeIn);
	Nc::Tween::Build(fadeInTween, &sprite.alpha, sprite.alpha, 1.0f, FADE_IN_TIME, CubicOut, FADE_OUT_DELAY);
	return entity;
}
