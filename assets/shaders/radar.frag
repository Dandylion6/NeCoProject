#version 330
precision highp float;

#define M_PI 3.1415926535897932384626433832795

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 outColor;

uniform sampler2D texture0;
uniform float time;
uniform float glitchStrength;

void main()
{
    float u = fragTexCoord.x;
    float v = fragTexCoord.y;

    if (glitchStrength > 0.0)
    {
        float t = time * 7.6;
        u += sin((v - t) * 0.7f * M_PI) * glitchStrength * 1.6;
        u -= sin((v - t - 104.018) * 2.0 * M_PI) * glitchStrength * 0.56;
        u += sin((v + t - 19.62) * 3.0 * M_PI) * glitchStrength * 0.3;

        v += sin((u - t + 28.035) * 0.8 * M_PI) * glitchStrength;
        v -= sin((u - t - 17.29) * 1.6 * M_PI) * glitchStrength * 0.64;
        v += sin((u + t + 56.679) * 5.6 * M_PI) * glitchStrength * 0.23;
    }

    vec3 textureColor = (texture(texture0, vec2(u, v)) * fragColor).rgb;

    int pixelSmear = 16;
    float distanceBleed = 0.8;

    for (int i = -pixelSmear; i < 0; ++i)
    {
        float x = max(u + (1.0 / 255.0 * i), 0.0);
        vec3 smearColor = (texture(texture0, vec2(x, v)) * fragColor).rgb;

        float factor = 1.0 - (abs(i) / float(pixelSmear));
        float bleedValue = factor * factor * distanceBleed;

        float lumninence = 0.2126 * smearColor.r + 0.7152 * smearColor.g + 0.0722 * smearColor.b;

        textureColor = mix(textureColor, smearColor, bleedValue * lumninence);
    }

    outColor = vec4(textureColor, 1.0);
}