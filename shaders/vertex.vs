#version 400 core

struct Light
{
    int lightType;
    vec3 lightPosition;
    vec3 lightDirection;
    vec4 lightColor;
    float lightIntensity;
    vec3 lightAttenuation;
    float spotLightAngle;
    float spotLightFalloff;
};

struct Varyings
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};

uniform mat4 mvpMatrix;
uniform int lightCount;
uniform Light lights[20];
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
out Varyings varyings;

void main()
{
    varyings.position = (mvpMatrix * vec4(vertexPosition, 1.0)).rgb;
    varyings.normal = (mvpMatrix * vec4(vertexNormal, 1.0)).rgb;
    varyings.uv = vertexUV;
    gl_Position = mvpMatrix * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
}