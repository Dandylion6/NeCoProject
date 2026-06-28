#pragma once
#include <array>

#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include  "game/contexts/scene_context.hpp"


namespace Object
{

class MorseMonitor final
{
public:
	// ------ Types ------
	struct Data final
	{
		entt::entity monitor = entt::null;
		entt::entity gauge = entt::null;
		entt::entity pointer = entt::null;


		[[nodiscard]] std::array<entt::entity, 3u> All() const noexcept
		{
			return { monitor, gauge, pointer };
		}
	};


	// ------ Members ------

	static constexpr Nc::Vector2f GAUGE_SIZE = Nc::Vector2f(8.0f, 74.0f);
	static constexpr Nc::Vector2f POSITION = Nc::Vector2f(440.0f, 60.0f);
    static constexpr float MARGIN_WIDTH = 1.0f;
    static constexpr float ROTATION = -8.0f;


	// ------ Functions ------

	static void Create(const SceneContext& context) noexcept;

private:
	class Gauge final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context) noexcept;

	};


	class Pointer final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(const SceneContext& context) noexcept;

	};


	class Region final
	{
	public:
		// ------ Functions ------
		static entt::entity Create(
			const SceneContext& context,
			Component::Morse::MonitorRegion::Region region
		) noexcept;

	};

};

}
