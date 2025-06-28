#include "assemblers/scenes/main_menu/menu_background_entity.h"
#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "components/ui/main_menu_tag.h"
#include "entt/entity/registry.hpp"
#include "utility/vector2.h"


const entt::entity Construct::MainMenuBackgroundEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

	registry.emplace<Tag::MainMenu>(entity);
	registry.emplace<Component::UiTransform>(entity, center, center, size);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}