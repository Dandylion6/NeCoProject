#pragma once
#include "core/data/vector2.hpp"
#include "game/state/game_state.hpp"
#include <cstdint>
#include <utility>

using SortIndex = int8_t;


namespace Component
{
	struct Transform
	{
		Nc::Vector2f position = Nc::Vector2f::Zero();
		Nc::Vector2f size = Nc::Vector2f::Zero();
		Nc::Vector2f offset = Nc::Vector2f::Zero();

		float rotation = 0.0f;
		Scene boundScene = CommsRoom;
		SortIndex index = 0;

		Transform() = default;
		Transform(
			Scene boundScene,
			Nc::Vector2f position = Nc::Vector2f::Zero(),
			Nc::Vector2f size = Nc::Vector2f::Zero(),
			Nc:: Vector2f offset = Nc::Vector2f::Zero(),
			SortIndex index = 0,
			float rotation = 0.0f
		): 
			boundScene(boundScene),
			position(position),
			size(size),
			offset(offset),
			index(index),
			rotation(rotation)
		{ };
	};
}


namespace Component::UI
{
	struct Transform
	{
		Nc::Vector2f anchor = Nc::Vector2f::Zero();
		Nc::Vector2f origin = Nc::Vector2f::Zero();
		Nc::Vector2f size = Nc::Vector2f::Zero();
		Nc::Vector2f offset = Nc::Vector2f::Zero();

		float rotation = 0.0f;
		SortIndex index = 0;
		bool isVisible = true;

		Transform() = default;
		Transform(
			Nc::Vector2f anchor,
			Nc::Vector2f origin = Nc::Vector2f::Zero(),
			Nc::Vector2f size = Nc::Vector2f::Zero(),
			Nc::Vector2f offset = Nc::Vector2f::Zero(),
			SortIndex index = 0,
			float rotation = 0.0f
		):
			anchor(anchor),
			origin(origin),
			size(size),
			offset(offset),
			index(index),
			rotation(rotation)
		{ };
	};
}