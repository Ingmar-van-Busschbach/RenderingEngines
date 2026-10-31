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

uniform CameraData cameraData;
uniform MatrixData matrixData;
uniform int lightCount;
uniform Light lights[20];
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
out Varyings varyings;

void main()
{
    varyings.positionWS = (matrixData.modelMatrix * vec4(vertexPosition, 1.0)).rgb;
    varyings.normalWS = (matrixData.modelMatrix * vec4(vertexNormal, 0.0)).rgb;
    varyings.uv = vertexUV;
    mat3 normalMatrix = mat3(transpose(inverse(matrixData.modelMatrix)));
    vec3 N = normalize (normalMatrix * vertexNormal);
    vec3 T = normalize (normalMatrix * vertexTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    varyings.TBN = mat3(T, B, N);
    gl_Position = matrixData.projectionMatrix * matrixData.viewMatrix * matrixData.modelMatrix * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
}