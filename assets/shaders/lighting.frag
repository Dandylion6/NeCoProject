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
uniform int lightTypes[MAX_LIGHT_SOURCES];
uniform vec2 lightDirections[MAX_LIGHT_SOURCES];
uniform float lightAngles[MAX_LIGHT_SOURCES];


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
    float diffuseReflectance = dot(normalVector, toLightDirection);

    float distance = length(lightPosition - pixelPosition);
    float attenuation = calculateAttentuation(distance, lightRange);

    float intensity = lightStrength * attenuation;
    return lightColor * diffuseReflectance * intensity;
}


float calculateBrightness(vec3 color)
{
    return (max(max(color.r, color.g), color.b) + min(min(color.r, color.g), color.b)) * 0.5;
}


void main()
{
    vec4 albedoColor = texture(albedo, fragTexCoord) * fragColor;
    vec4 normalsColor = texture(normals, fragTexCoord);
    vec4 aoColor = texture(ao, fragTexCoord);

    vec3 normalVector = convertNormalMap(normalsColor);

    vec2 pixelPosition = gl_FragCoord.xy;
    vec3 pixelPos3D = vec3(pixelPosition, 0.0);

    vec3 ambient = vec3(0.02, 0.13, 0.19);
    vec3 diffuse = ambient;

    for (int i = 0; i < lightPointCount; i++)
    {
        diffuse += calculatePointLight(i, pixelPos3D, normalVector);
    }

    float occlusion = 1.0 - calculateBrightness(aoColor.rgb);
    occlusion = 1.0 - pow(occlusion, 1.6);

    diffuse *= occlusion;
    diffuse = mix(ambient, diffuse, calculateBrightness(diffuse));

    vec3 finalColor = albedoColor.rgb * diffuse;
    outColor = vec4(finalColor, albedoColor.a);
}