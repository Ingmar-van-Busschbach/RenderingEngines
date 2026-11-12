#version 400 core
#define PI 3.1415926535897932384626433832795

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
    sampler2D metallicTexture;
    sampler2D roughnessTexture;
    vec3 albedoColor;
    float specularIntensity;
    float metallic;
    float roughness;
};

uniform Material material;
uniform CameraData cameraData;
uniform MatrixData matrixData;
uniform int lightCount;
uniform Light lights[20];
in Varyings varyings;

out vec4 FragColor;

vec3 schlickFresnel(float vDotH, vec3 albedo, float metallic)
{
    vec3 F0 = vec3(0.04);
    if(metallic > 0){
        F0 = mix(F0, albedo, metallic);
    }

    vec3 ret = F0 + (1 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5);

    return ret;
}

float ggxDistribution(float nDotH, float Roughness)
{
    float alpha2 = Roughness * Roughness * Roughness * Roughness;
    float d = nDotH * nDotH * (alpha2 - 1) + 1;
    float ggxdistrib = alpha2 / (PI * d * d);
    return ggxdistrib;
}

float geomSmith(float dp, float Roughness)
{
    float k = (Roughness + 1.0) * (Roughness + 1.0) / 8.0;
    float denom = dp * (1 - k) + k;
    return dp / denom;
}

vec3 CalcPBRLighting(Light light, vec3 normal, vec3 albedo, float metallic, float roughness){
    vec3 lightIntensity = light.lightColor.rgb * light.lightIntensity;

    vec3 l = vec3(0.0);

    if(light.lightType == 0){
        l = light.lightDirection;
    }
    else{
        l = light.lightPosition - varyings.positionWS;
        l = normalize(l);
    }
    float distance = length(light.lightPosition - varyings.positionWS);
    float attenuation = light.lightAttenuation.r + light.lightAttenuation.g * distance + light.lightAttenuation.b * distance * distance;
    if(attenuation > 0){
        lightIntensity /= attenuation;
    }

    vec3 n = normal;
    vec3 v = normalize(cameraData.cameraPosition - varyings.positionWS);
    vec3 h = normalize(v + l);

    float nDotH = max(dot(n, h), 0.0);
    float vDotH = max(dot(v, h), 0.0);
    float nDotL = max(dot(n, l), 0.0);
    float nDotV = max(dot(n, v), 0.0);

    vec3 F = schlickFresnel(vDotH, albedo, metallic);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    float SpecBDRF_nom = ggxDistribution(nDotH, roughness) * geomSmith(nDotL, roughness) * geomSmith(nDotV, roughness);
    float SpecBDRF_denom = 4.0 * nDotV * nDotL + 0.001;

    float SpecBDRF = SpecBDRF_nom / SpecBDRF_denom;

    vec3 fLambert = vec3(0.0);

    if(metallic < 1){
        fLambert = mix(albedo, vec3(0.0), metallic);
    }

    vec3 DiffuseBRDF = kD * fLambert / PI;

    return (DiffuseBRDF + SpecBDRF) * lightIntensity * nDotL;
}

vec4 CalcTotalPBRLighting()
{
    vec3 finalColor = vec3(0,0,0);
    vec4 objectColor = vec4(material.albedoColor, 1);
    vec3 normal = normalize(varyings.normalWS);
    float metallic = material.metallic;
    float roughness = material.roughness;
    if(material.materialType == 3 || material.materialType == 5)
    {
        objectColor = texture(material.albedoTexture, varyings.uv);
    }
    if(material.materialType == 3)
    {
        normal = texture(material.normalTexture, varyings.uv).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(varyings.TBN * normal);
        metallic = texture(material.metallicTexture, varyings.uv).r;
        roughness = texture(material.roughnessTexture, varyings.uv).r;
    }
    for(int i = 0; i < lightCount; i++){
        if(lights[i].lightType == 2){
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
                      vec3 color = CalcPBRLighting(lights[i], normal, objectColor.rgb, metallic, roughness);
                      color *= angleRatio;
                      finalColor += color;
                  }
        }
        else if(lights[i].lightType == 3){
            finalColor += lights[i].lightColor.rgb * lights[i].lightIntensity * 0.01;
        }
        else{
            finalColor += CalcPBRLighting(lights[i], normal, objectColor.rgb, metallic, roughness);
        }
    }

    // HDR Tone Mapping
    finalColor = finalColor / (finalColor + vec3(1.0));

    // Gamma Correction
    return vec4(pow(finalColor, vec3(1.0/2.2)), 1.0);
}

void main()
{
    FragColor = CalcTotalPBRLighting();
}