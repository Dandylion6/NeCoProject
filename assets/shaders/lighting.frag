#version 330
precision highp float;


in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 outColor;

uniform sampler2D albedo;
uniform sampler2D normals;
uniform sampler2D ao;

#define MAX_LIGHT_SOURCES 8
uniform int lightPointCount;
uniform vec3 lightPositions[MAX_LIGHT_SOURCES];
uniform vec4 lightColors[MAX_LIGHT_SOURCES];
uniform float lightRanges[MAX_LIGHT_SOURCES];
uniform float lightStrengths[MAX_LIGHT_SOURCES];
uniform float ambientTint;


vec3 convertNormalMap(vec4 normalColor)
{
    return normalize(normalColor.rgb * 2.0 - 1.0);
}


float calculateAttentuation(float distance, float radius)
{
    float factor = distance / radius;
    float attenuation = clamp(1.0 - factor * factor, 0.0, 1.0);
    return attenuation * attenuation;
}


vec3 calculatePointLight(int light, vec3 pixelPosition, vec3 normalVector)
{
    vec3 lightPosition = vec3(lightPositions[light]);
    vec3 lightColor = lightColors[light].rgb;
    float lightStrength = lightStrengths[light];
    float lightRange = lightRanges[light];

    vec3 toLightDirection = normalize(lightPosition - pixelPosition);
    float diffuseReflectance = max(dot(normalVector, toLightDirection), 0.0);

    float distance = length(lightPosition - pixelPosition);
    float attenuation = calculateAttentuation(distance, lightRange);

    float intensity = lightStrength * attenuation;
    return lightColor * diffuseReflectance * intensity;
}


float calculateBrightness(vec3 color)
{
    return (max(max(color.r, color.g), color.b) + min(min(color.r, color.g), color.b)) * 0.5;
}


float bayerDither2x2(vec2 texelCoord)
{
    int x = int(mod(texelCoord.x, 2.0));
    int y = int(mod(texelCoord.y, 2.0));
    int index = x + y * 2;

    // thresholds 0, 2, 3, 1 normalized to 0..1
    float bayer[4] = float[4](0.0, 0.5, 0.75, 0.25);
    return bayer[index];
}


void main()
{
    vec4 albedoColor = texture(albedo, fragTexCoord) * fragColor;
    vec4 normalsColor = texture(normals, fragTexCoord);
    vec4 aoColor = texture(ao, fragTexCoord);

    vec3 normalVector = convertNormalMap(normalsColor);

    vec2 pixelPosition = gl_FragCoord.xy;
    vec3 pixelPos3D = vec3(pixelPosition, 0.0);

    vec3 ambient = vec3(0.02, 0.16, 0.21);
    vec3 diffuse = ambient;

    for (int i = 0; i < lightPointCount; i++)
    {
        diffuse += calculatePointLight(i, pixelPos3D, normalVector);
    }

    float occlusion = 1.0 - calculateBrightness(aoColor.rgb);
    occlusion = 1.0 - pow(occlusion, 1.7);

    diffuse *= occlusion;
    float diffuseBrightness = calculateBrightness(diffuse);

    vec2 textureSize = vec2(textureSize(albedo, 0));
    vec2 texelCoord = floor(fragTexCoord * textureSize);
    float ditherThreshold = bayerDither2x2(texelCoord);

    diffuseBrightness = floor(diffuseBrightness * 5.0 + ditherThreshold) / 5.0;
    diffuse = mix(ambient, diffuse, diffuseBrightness);

    vec3 finalColor = albedoColor.rgb * diffuse;
    outColor = vec4(finalColor, albedoColor.a);
}