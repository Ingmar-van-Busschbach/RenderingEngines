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
uniform sampler2D text;
in Varyings varyings;

out vec4 FragColor;

void main()
{
    float specularPower = 256;
    vec4 objectColor = texture(text, varyings.uv);
    vec3 finalColor = vec3(0,0,0);
    vec3 cameraDirection = normalize(varyings.positionWS - cameraData.cameraPosition);
    for(int i = 0; i < lightCount; i++)
    {
        vec3 ambientColor = vec3(0,0,0);
        vec3 diffuseColor = vec3(0,0,0);
        vec3 specularColor = vec3(0,0,0);
        //Directional
        if(lights[i].lightType == 0)
        {
            diffuseColor += max(dot(normalize(lights[i].lightDirection), varyings.normalWS),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
            vec3 reflection = reflect(lights[i].lightDirection, varyings.normalWS);
            specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Point
        if(lights[i].lightType == 1)
        {
            vec3 lightDirection = normalize(lights[i].lightPosition - varyings.positionWS);
            diffuseColor += max(dot(lightDirection, varyings.normalWS),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
            vec3 reflection = reflect(lightDirection, varyings.normalWS);
            specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Spot
        if(lights[i].lightType == 2)
        {
            vec3 lightDirection = normalize(lights[i].lightPosition - varyings.positionWS);
            float angle = (dot(lightDirection, varyings.normalWS) - 1) * -90;
            if(angle < lights[i].spotLightAngle)
            {
                float angleRatio = 0;
                if(lights[i].spotLightAngle - lights[i].spotLightFalloff > 0)
                {
                    angleRatio = max(angle - lights[i].spotLightFalloff / lights[i].spotLightAngle - lights[i].spotLightFalloff,0);
                }
                angleRatio = 1 - angleRatio;
                diffuseColor += max(dot(lightDirection, varyings.normalWS),0) * lights[i].lightIntensity * angleRatio * lights[i].lightColor.rgb * objectColor.rgb;
                vec3 reflection = reflect(lightDirection, varyings.normalWS);
                specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
            }
        }
        //Ambient
        if(lights[i].lightType == 3)
        {
            ambientColor += lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
        }
        float distance = length(lights[i].lightPosition - varyings.positionWS);
        float attenuation = lights[i].lightAttenuation.r + lights[i].lightAttenuation.g * distance + lights[i].lightAttenuation.b * distance * distance;
        if(attenuation > 0)
        {
            finalColor += ambientColor + (diffuseColor + specularColor) / attenuation;
        }
        else
        {
            finalColor += ambientColor + diffuseColor + specularColor;
        }

    }
   FragColor = vec4(finalColor, 1);
}