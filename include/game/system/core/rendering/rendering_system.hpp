#pragma once
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
struct RenderContext;
class ResourceStore;


class RenderingSystem
{
public:
	static void DrawScreen(
		entt::registry& registry,
		RenderContext& renderContext,
		GameState& gameState,
		Nc::Vector2f cameraPosition
	);

	static void DrawUi(
		entt::registry& registry,
		ResourceStore& resourceStore,
		RenderContext& renderContext,
		GameState& gameState
	);

private:
	enum class RenderType: uint8_t
	{
		Invalid,
		Sprite,
		Rectangle,
		Text,
	};

	struct Renderable
    {
        entt::entity entity = entt::null;
        RenderType type = RenderType::Invalid;
        SortIndex index = 0;
    };

	static RenderType GetRenderType(entt::registry& registry, const entt::entity entity);

	static bool ShouldRender(const Component::Transform& transform, Scene currentScene);

	static bool ShouldRender(const Component::UiTransform& transform);

	static bool SortComparison(Renderable a, Renderable b);
};