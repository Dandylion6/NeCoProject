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
uniform vec3 lightPosition[MAX_LIGHT_SOURCES];
uniform vec4 lightColor[MAX_LIGHT_SOURCES];
uniform float lightRange[MAX_LIGHT_SOURCES];
uniform float lightStrength[MAX_LIGHT_SOURCES];
uniform int lightTypes[MAX_LIGHT_SOURCES];
uniform vec2 lightDirections[MAX_LIGHT_SOURCES];
uniform float lightAngles[MAX_LIGHT_SOURCES];


void main()
{
    vec4 albedoColor = texture(albedo, fragTexCoord) * fragColor;
    vec4 normalsColor = texture(normals, fragTexCoord);
    vec4 aoColor = texture(ao, fragTexCoord);

    vec3 normalVector = normalize(normalsColor.rgb * 2.0 - 1.0);

    vec3 diffuse = vec3(0.3);
    vec2 pixelPosition = gl_FragCoord.xy;

    for (int i = 0; i < lightPointCount; i++)
    {
        vec3 lightPos3D = vec3(lightPosition[i]);
        vec3 pixelPos3D = vec3(pixelPosition, 0.0);
        vec3 dirToLight = normalize(lightPos3D - pixelPos3D);

        // Standard Lambertian Lighting
        float diffuseReflectance = dot(normalVector, dirToLight);

        // Attenuation: Smoother, more realistic decay
        float distance = length(lightPos3D - pixelPos3D);
        float d = distance / lightRange[i];
        float attenuation = 1.0 / (1.0 + 2.0*d + 10.0*d*d);

        // Mask the attenuation so it hits 0 at the light's range
        attenuation *= smoothstep(1.0, 0.8, d);

        vec3 intensity = lightColor[i].rgb * lightStrength[i] * attenuation;
        diffuse += intensity * diffuseReflectance;
    }

    // Final composition
    vec3 occlusion = aoColor.rgb * 0.6 + 0.4;
    vec3 finalColor = albedoColor.rgb * diffuse * occlusion;
    outColor = vec4(finalColor, albedoColor.a);
}