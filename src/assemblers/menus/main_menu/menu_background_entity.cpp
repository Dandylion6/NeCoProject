#include "assemblers/menus/main_menu/menu_background_entity.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "components/ui/main_menu_tag.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"


const entt::entity Construct::MainMenuBackgroundEntity(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::MainMenu>(entity);
	registry.emplace<Component::UiTransform>(entity, center, center, size);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}