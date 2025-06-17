#pragma once
#include "core/scene.h"
#include "utility/vector2.h"
#include <cstdint>

using zIndex = int8_t;


namespace Component
{
	struct Transform
	{
		Nc::Vector2f position = Nc::Vector2f::Zero();
		Nc::Vector2f size = Nc::Vector2f::Zero();
		Nc::Vector2f offset = Nc::Vector2f::Zero();

		float rotation = 0.0f;
		Scene boundScene = CommsRoom;
		zIndex index = 0;

		Transform() = default;
		Transform(
			Scene boundScene,
			Nc::Vector2f position = Nc::Vector2f::Zero(),
			Nc::Vector2f size = Nc::Vector2f::Zero(),
			Nc:: Vector2f offset = Nc::Vector2f::Zero(),
			float rotation = 0.0f,
			zIndex index = 0
		): 
			boundScene(boundScene),
			position(position),
			size(size),
			offset(offset),
			rotation(rotation),
			index(index)
		{ };
	};


	struct UiTransform
	{
		Nc::Vector2f anchor = Nc::Vector2f::Zero();
		Nc::Vector2f origin = Nc::Vector2f::Zero();
		Nc::Vector2f size = Nc::Vector2f::Zero();
		Nc::Vector2f offset = Nc::Vector2f::Zero();

		float rotation = 0.0f;
		zIndex index = 0;

		UiTransform() = default;
		UiTransform(
			Nc::Vector2f anchor,
			Nc::Vector2f origin = Nc::Vector2f::Zero(),
			Nc::Vector2f size = Nc::Vector2f::Zero(),
			Nc::Vector2f offset = Nc::Vector2f::Zero(),
			float rotation = 0.0f,
			zIndex index = 0
		):
			anchor(anchor),
			origin(origin),
			size(size),
			offset(offset),
			rotation(rotation),
			index(index)
		{ };
	};
}