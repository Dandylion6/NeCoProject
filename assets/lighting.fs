#version 330
precision highp float;


in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 outColor;

uniform sampler2D texture0;


#define MAX_LIGHT_SOURCES 8
uniform int lightSourceCount;
uniform vec3 lightPosition[MAX_LIGHT_SOURCES];
uniform vec4 lightColor[MAX_LIGHT_SOURCES];
uniform float lightRange[MAX_LIGHT_SOURCES];
uniform float lightStrength[MAX_LIGHT_SOURCES];
uniform int lightTypes[MAX_LIGHT_SOURCES];
uniform vec2 lightDirections[MAX_LIGHT_SOURCES];
uniform float lightAngles[MAX_LIGHT_SOURCES];


void main()
{
    vec4 textureColor = texture(texture0, fragTexCoord);
    vec3 finalColor = vec3(0.2, 0.2, 0.2);
    vec2 pixelPosition = gl_FragCoord.xy;

    for (int i = 0; i < lightSourceCount; i++)
    {
        float distanceToSource = distance(pixelPosition, lightPosition[i].xy);
        float falloff = distanceToSource / lightRange[i];
        float lightIntensity = 1.0 - smoothstep(0.0, 1.0, falloff * falloff);
        vec4 finalLightColor = lightColor[i] * lightIntensity * lightStrength[i];

        finalColor.rgb += finalLightColor.rgb;
    }

    outColor = vec4(textureColor.rgb * finalColor.rgb, textureColor.a);
}