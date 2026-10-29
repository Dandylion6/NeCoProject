#include "game/construction/scene/comms_scene/object/blip_object.hpp"
#include "raylib.h"
#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"
#include "game/utility/color_palette.hpp"


Object::Blip::Data Object::Blip::Create(const SceneContext& context, Nc::Vector2f position, int16_t health) noexcept
{
    constexpr auto CENTER = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;
    constexpr char PING_PATH[] = "assets/audio/object/radar/blip_ping.wav";
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/display/radar_blip.png";
	constexpr float FADE_IN_TIME = 0.3f, FADE_OUT_TIME = 3.8f;
	constexpr float FADE_OUT_DELAY = 1.8f;

	const entt::entity entity = context.registry.create();
    const entt::entity emitter = context.registry.create();

	context.registry.emplace<Component::Blip>(entity, emitter);
	context.registry.emplace<Component::Health>(entity, health);

	const Texture2D& texture = context.store.GetTexture(FILE_PATH);
	auto& sprite = context.registry.emplace<Component::Sprite>(entity, texture, 0.0f);

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

    context.registry.emplace<Component::Transform>(emitter, CommsRoom, CENTER, size, size * 0.5f);

    const Sound& ping = context.store.CreateSoundHandle(PING_PATH);
    context.registry.emplace<Component::ShotAudio>(emitter, ping);
    context.registry.emplace<Component::AudioEmitter>(emitter, 0.1f);

	return { .blip = entity, .emitter = emitter };
}
