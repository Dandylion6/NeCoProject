#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/entity/menu_background_entity.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/main_menu_tag.hpp"
#include "raylib.h"
#include <utility>


const entt::entity Construct::MainMenuBackgroundEntity(
	entt::registry& registry,
	Nc::ResourceStore& resourceStore
)
{
	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::MainMenu>(entity);
	registry.emplace<Component::UI::Transform>(entity, center, center, size);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}