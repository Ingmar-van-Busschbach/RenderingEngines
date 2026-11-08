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
    vec3 positionWS;
    vec3 normalWS;
    vec2 uv;
    mat3 TBN;
};

struct MatrixData
{
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

struct CameraData
{
    vec3 cameraPosition;
};

struct Material
{
    int materialType;
    sampler2D albedoTexture;
    sampler2D specularTexture;
    sampler2D normalTexture;
    vec3 albedoColor;
    float specularIntensity;
};

uniform Material material;
uniform CameraData cameraData;
uniform MatrixData matrixData;
uniform int lightCount;
uniform Light lights[20];
in Varyings varyings;

out vec4 FragColor;

void main()
{
   FragColor = vec4(1,1,1,1);
}