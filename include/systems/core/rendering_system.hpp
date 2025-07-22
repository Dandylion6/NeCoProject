#pragma once
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
struct RenderContext;


class RenderingSystem
{
public:
	static void Draw(
		entt::registry& registry,
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

	static RenderType GetRenderType(entt::registry& registry, const entt::entity entity);

	static bool ShouldRender(const Component::Transform& transform, Scene currentScene);

	static bool ShouldRender(const Component::UiTransform& transform);
};