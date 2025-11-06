#include "game/construction/ui/restart_menu/entity/restart_background_entity.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/utility/color_palette.hpp"
#include "core/data/vector2.hpp"
#include <utility>


namespace Construct
{
	static const entt::entity RestartBackdropEntity(
		entt::registry& registry, Nc::Vector2f windowSize
	)
	{
		const entt::entity entity = registry.create();

		Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

		registry.emplace<Tag::DontDestroyOnLoad>(entity);
		registry.emplace<Tag::RestartMenu>(entity);
		registry.emplace<Component::UI::Transform>(entity, center, center, windowSize);
		registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

		return entity;
	};
};


const entt::entity Construct::RestartMenuBackgroundEntity(
	entt::registry& registry, Nc::ResourceStore& resourceStore, Nc::Vector2f windowSize
)
{
	Construct::RestartBackdropEntity(registry, windowSize);

	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::RestartMenu>(entity);
	registry.emplace<Component::UI::Transform>(entity, center, center, size, Nc::Vector2f::Zero(), 1);
	registry.emplace<Component::Sprite>(entity, std::move(texture), 0.4f);

	return entity;
}