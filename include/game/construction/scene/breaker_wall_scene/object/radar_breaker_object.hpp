#pragma once
#include <array>

#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/shared/object/mechanical/circuit_breaker_object.hpp"
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
	    entt::entity indicator = entt::null;

		[[nodiscard]] std::array<entt::entity, 3u> All() const noexcept { return { base, lever, indicator }; }
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
		static CircuitBreaker::Data Create(const SceneContext& context, entt::entity radar, entt::entity base);

	};


    class Indicator final
    {
    public:
        // ------ Functions ------
        static entt::entity Create(const SceneContext& context, entt::entity indicator);

    };

};

}
