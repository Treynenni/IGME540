#ifndef __GGP_LIGHTS__ // Each .hlsli file needs a unique identifier!
#define __GGP_LIGHTS__

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#define MAX_SPECULAR_EXPONENT 256.0f

struct Light
{
    int Type;
    float3 Direction;
    
    float Range;
    float3 Position;
    
    float Intensity;
    float3 Color;
    
    float SpotInnerAngle;
    float SpotOuterAngle;
    float2 Padding;
};

float Diffuse(float3 normal, float3 lightDirection)
{
    return saturate(dot(normal, lightDirection));
}

float Specular(float3 normal, float3 lightDirection, float3 worldPos, float3 camPos, float roughness)
{
    float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
    
    if (specExponent < 0.05f)
    {
        return 0;
    }
    
    float3 view = normalize(camPos - worldPos);
    float3 reflection = reflect(-lightDirection, normal);
    
    return pow(max(dot(reflection, view), 0.0f), specExponent);
}

float Attenuate(Light light, float3 worldPos)
{
    float dist = distance(light.Position, worldPos);
    float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
    return att * att;
}

float3 DirectionalLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float3 surfaceColor)
{
    float3 lightDirection = normalize(-light.Direction);
    
    float diffusion = Diffuse(normal, lightDirection);
    float specular = Specular(normal, lightDirection, worldPos, camPos, roughness);

    return (diffusion + specular) * surfaceColor * light.Intensity * light.Color;
}

float3 PointLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float3 surfaceColor)
{
    float3 lightDirection = normalize(light.Position - worldPos);
    
    float diffusion = Diffuse(normal, lightDirection);
    float specular = Specular(normal, lightDirection, worldPos, camPos, roughness);
    float attenuate = Attenuate(light, worldPos);
    
    return (diffusion + specular) * surfaceColor * attenuate * light.Intensity * light.Color;
}

float3 SpotLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float3 surfaceColor)
{
    float3 lightDirection = normalize(light.Position - worldPos);
    float angle = dot(-lightDirection, normalize(light.Direction));

    float outer = cos(light.SpotOuterAngle);
    float inner = cos(light.SpotInnerAngle);
    float spotTerm = saturate((angle - outer) / (inner - outer));

    float diffusion = Diffuse(normal, lightDirection);
    float specular = Specular(normal, lightDirection, worldPos, camPos, roughness);
    float attenuation = Attenuate(light, worldPos);

    return (diffusion + specular) * surfaceColor * attenuation * spotTerm * light.Intensity * light.Color;
}

#endif