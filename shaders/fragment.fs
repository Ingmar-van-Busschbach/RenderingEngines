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
    //Declare base variables
    vec3 finalColor = vec3(0,0,0);
    vec4 objectColor = vec4(1,1,1,1);
    float specularPower = 256;
    vec3 normal = normalize(varyings.normalWS);

    //Load data from material
    if(material.materialType == 0 || material.materialType == 2)
    {
        objectColor = texture(material.albedoTexture, varyings.uv);
    }
    else
    {
        objectColor = vec4(material.albedoColor, 1);
    }
    if(material.materialType == 0)
    {
        specularPower *= texture(material.specularTexture, varyings.uv).r;
        normal = texture(material.normalTexture, varyings.uv).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(varyings.TBN * normal);
    }
    else
    {
        specularPower *= material.specularIntensity;
    }

    //Light Calculation
    vec3 cameraDirection = normalize(varyings.positionWS - cameraData.cameraPosition);
    for(int i = 0; i < lightCount; i++)
    {
        vec3 ambientColor = vec3(0,0,0);
        vec3 diffuseColor = vec3(0,0,0);
        vec3 specularColor = vec3(0,0,0);
        //Directional
        if(lights[i].lightType == 0)
        {
            diffuseColor += max(dot(normalize(lights[i].lightDirection), normal),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
            vec3 reflection = reflect(lights[i].lightDirection, normal);
            specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Point
        else if(lights[i].lightType == 1)
        {
            vec3 lightDirection = normalize(lights[i].lightPosition - varyings.positionWS);
            diffuseColor += max(dot(lightDirection, normal),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
            vec3 reflection = reflect(lightDirection, normal);
            specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Spot
        else if(lights[i].lightType == 2)
        {
            vec3 lightDirection = normalize(lights[i].lightPosition - varyings.positionWS);
            float angle = (dot(lightDirection, normal) -1) * -90; // convert -1...1 to 180...0 to calculate the angle between the two directions
            if(angle < lights[i].spotLightAngle)
            {
                float angleRatio = 0;
                if(lights[i].spotLightAngle - lights[i].spotLightFalloff > 0)
                {
                    angleRatio = max(angle - lights[i].spotLightFalloff / lights[i].spotLightAngle - lights[i].spotLightFalloff,0);
                }
                angleRatio = 1 - angleRatio;
                diffuseColor += max(dot(lightDirection, normal),0) * lights[i].lightIntensity * angleRatio * lights[i].lightColor.rgb * objectColor.rgb;
                vec3 reflection = reflect(lightDirection, normal);
                specularColor += pow(max(dot(reflection, cameraDirection),0), specularPower) * lights[i].lightColor.rgb * objectColor.rgb;
            }
        }
        //Ambient
        else if(lights[i].lightType == 3)
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