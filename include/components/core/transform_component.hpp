#pragma once
#include "core/scene.hpp"
#include "utility/vector2.hpp"
#include <cstdint>

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


	struct UiTransform
	{
		Nc::Vector2f anchor = Nc::Vector2f::Zero();
		Nc::Vector2f origin = Nc::Vector2f::Zero();
		Nc::Vector2f size = Nc::Vector2f::Zero();
		Nc::Vector2f offset = Nc::Vector2f::Zero();

		float rotation = 0.0f;
		SortIndex index = 0;
		bool isVisible = true;

		UiTransform() = default;
		UiTransform(
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