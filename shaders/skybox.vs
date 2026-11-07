#version 330 core
struct MatrixData
{
   mat4 modelMatrix;
   mat4 viewMatrix;
   mat4 projectionMatrix;
};

uniform MatrixData matrixData;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;

out vec3 texCoords;

void main()
{
    vec4 pos = matrixData.projectionMatrix * matrixData.viewMatrix * vec4(vertexPosition, 1.0f);
    // Having z equal w will always result in a depth of 1.0f
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    // We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
    texCoords = vec3(vertexPosition.x, vertexPosition.y, -vertexPosition.z);
}