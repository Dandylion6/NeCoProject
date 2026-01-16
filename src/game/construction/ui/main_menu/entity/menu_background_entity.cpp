#include "game/construction/ui/main_menu/entity/menu_background_entity.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/main_menu_tag.hpp"


entt::entity Entity::MainMenuBackground::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f CENTER = Nc::Vector2f::Scale(0.5f);
	constexpr char FILE_DIRECTORY[] = "assets/environment/backgrounds/main_menu.png";

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
	context.registry.emplace<Tag::MainMenu>(entity);

	Texture2D texture = context.store.GetTexture(FILE_DIRECTORY);
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);

	context.registry.emplace<Component::UI::Transform>(entity, CENTER, CENTER, size);
	context.registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}
