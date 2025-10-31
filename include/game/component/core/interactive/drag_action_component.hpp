#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"


namespace Component
{
	struct DragAction
	{
		Nc::Vector2f startPosition = Nc::Vector2f::Zero();
		Nc::Vector2f draggedDelta = Nc::Vector2f::Zero();
		bool isTarget = false;

		DragAction() = default;
	};
}