#pragma once
#include <array>

#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"


namespace Component
{
namespace Action { struct Toggle; }
struct Radar;
};
struct SceneContext;


namespace Object
{

class Radar final
{
public:
	// ------ Types ------
	struct Data final
	{
		entt::entity radar = entt::null;
		entt::entity path = entt::null;
		entt::entity artillery = entt::null;
		entt::entity errorWarning = entt::null;
		entt::entity recalibrationText = entt::null;
		entt::entity powerButton = entt::null;


		[[nodiscard]] std::array<entt::entity, 6u> All() const noexcept
		{
			return { radar, path, artillery, errorWarning, recalibrationText, powerButton };
		}
	};


	// ------ Functions ------

	static Data Create(const SceneContext& context) noexcept;

private:
	class Path final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context) noexcept;

	};


	class Artillery final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context) noexcept;

	};


	class ErrorWarning final
	{
	public:
		// ------ Functions -------
		static entt::entity Create(entt::registry& registry) noexcept;

	};


	class RecalibrationText final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(entt::registry& registry) noexcept;

	};


	class PowerButton final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(
			entt::registry& registry
		) noexcept;

	};

};

}
