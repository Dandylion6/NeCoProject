#pragma once
#include "raylib.h"


namespace Component
{
/**
 * @brief Stores the texture and alpha value to render.
 * 
 * Usage example:
 * @code
 * Texture2D texture = resourceStore.GetTexture("/././.png");
 * 
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Sprite>(entity, std::move(texture));
 * @endcode
 */
struct Sprite final
{
    // ------ Members ------

    Texture2D albedo{ };
    Texture2D normals{ };
    Texture2D ambientOcclusion{ };
    float alpha = 1.0f;
    float scale = 1.0f;


    // ------ Constructors ------

    explicit Sprite(const Texture2D& albedo, const float alpha = 1.0f) noexcept
        : albedo(albedo), alpha(alpha)
    {
        constexpr Color BASE_NORMAL_COLOR = {128,128,255, 255};
        constexpr Color BASE_AMBIENT_COLOR = {255,255,255,255};

        const Image baseNormals = GenImageColor(albedo.width, albedo.height, BASE_NORMAL_COLOR);
        normals = LoadTextureFromImage(baseNormals);

        const Image baseAO = GenImageColor(albedo.width, albedo.height, BASE_AMBIENT_COLOR);
        ambientOcclusion = LoadTextureFromImage(baseAO);
    }

    explicit constexpr Sprite(
        const Texture2D& albedo,
        const Texture2D& normals,
        const Texture2D& ao,
        const float alpha = 1.0f
    ) noexcept
        : albedo(albedo), normals(normals), ambientOcclusion(ao), alpha(alpha) {}
};
}
