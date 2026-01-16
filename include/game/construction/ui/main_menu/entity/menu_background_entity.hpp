#pragma once
#include "entt/entity/fwd.hpp"
#include "game/contexts/scene_context.hpp"


namespace Entity
{

	class MainMenuBackground final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context) noexcept;

	};

}