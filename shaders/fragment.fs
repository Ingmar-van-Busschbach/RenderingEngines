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
uniform int lightCount;
uniform Light lights[20];
uniform sampler2D text;
in Varyings varyings;

out vec4 FragColor;


void main()
{
    vec4 objectColor = texture(text, varyings.uv);
    vec3 ambientColor = vec3(0,0,0);
    vec3 diffuseColor = vec3(0,0,0);
    vec3 finalColor = vec3(0,0,0);
    for(int i = 0; i < lightCount; i++)
    {
        //Directional
        if(lights[i].lightType == 0)
        {
            diffuseColor += max(dot(normalize(lights[i].lightDirection), varyings.normal),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Point
        if(lights[i].lightType == 1)
        {
            vec3 direction = normalize(lights[i].lightPosition - varyings.position);
            diffuseColor += max(dot(direction, varyings.normal),0) * lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
        }
        //Spot
        if(lights[i].lightType == 2)
        {
            vec3 direction = normalize(lights[i].lightPosition - varyings.position);
            float angle = (dot(direction, varyings.normal) - 1) * -90;
            if(angle < lights[i].spotLightAngle)
            {
                float angleRatio = 0;
                if(lights[i].spotLightAngle - lights[i].spotLightFalloff > 0)
                {
                    angleRatio = max(angle - lights[i].spotLightFalloff / lights[i].spotLightAngle - lights[i].spotLightFalloff,0);
                }
                angleRatio = 1 - angleRatio;
                diffuseColor += max(dot(direction, varyings.normal),0) * lights[i].lightIntensity * angleRatio * lights[i].lightColor.rgb * objectColor.rgb;
            }
        }
        //Ambient
        if(lights[i].lightType == 3)
        {
            ambientColor += lights[i].lightIntensity * lights[i].lightColor.rgb * objectColor.rgb;
        }
        finalColor += ambientColor + diffuseColor;
    }
   FragColor = vec4(finalColor, 1);
}