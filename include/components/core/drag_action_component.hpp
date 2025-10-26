#pragma once
#include "entt/entity/fwd.hpp"
#include "entt/signal/delegate.hpp"
#include "utility/vector2.hpp"
#include <utility>


namespace Component
{
	struct DragAction
	{
		using DragDelegate = entt::delegate<void(entt::registry&, const entt::entity)>;


		Nc::Vector2f originPosition = Nc::Vector2f::Zero();
		bool isTarget = false;
		DragDelegate onDragging { };
		DragDelegate onReleased { };


		DragAction() = default;
		DragAction(DragDelegate&& onDragging, DragDelegate&& onReleased) : 
			onDragging(std::move(onDragging)), onReleased(std::move(onReleased)) { };
	};
}