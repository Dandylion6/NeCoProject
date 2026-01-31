#pragma once
#include <array>

#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/contexts/scene_context.hpp"


namespace Object
{

class RadarBreaker final
{
public:
	// ------ Types ------
	struct Data final
	{
		entt::entity base = entt::null;
		entt::entity lever = entt::null;

		[[nodiscard]] std::array<entt::entity, 2u> All() const noexcept { return { base, lever }; }
	};


	// ------ Functions ------

	static void Create(const SceneContext& context, entt::entity radar);

private:
	class LeverBase final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context);

	};


	class LeverHandle final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context, entt::entity radar);

	};

};

}
