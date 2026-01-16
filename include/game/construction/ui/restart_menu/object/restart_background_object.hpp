#pragma once
#include "core/data/vector2.hpp"
#include "game/contexts/scene_context.hpp"
#include "entt/entity/fwd.hpp"


namespace Object
{

class RestartMenuBackground final
{
public:
	static void Create(SceneContext context, Nc::Vector2f windowSize) noexcept;

private:
	class FrontTexture final
	{
	public:
		static entt::entity Create(SceneContext context) noexcept;

	};


	class Backdrop final
	{
	public:
		static entt::entity Create(entt::registry& registry, Nc::Vector2f windowSize) noexcept;

	};

};

}