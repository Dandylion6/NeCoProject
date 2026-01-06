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


entt::entity Entity::MainMenuBackground::Create(
	entt::registry& registry, Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr Nc::Vector2f CENTER = Nc::Vector2f::Scale(0.5f);
	
	entt::entity entity = registry.create();

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::MainMenu>(entity);

	Texture2D texture = resourceStore.GetTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);

	registry.emplace<Component::UI::Transform>(entity, CENTER, CENTER, size);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}