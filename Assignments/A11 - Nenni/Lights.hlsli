#ifndef __GGP_LIGHTS__ // Each .hlsli file needs a unique identifier!
#define __GGP_LIGHTS__

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#define MAX_SPECULAR_EXPONENT 256.0f

static const float PI = 3.14159265359f;

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

//float Specular(float3 normal, float3 lightDirection, float3 worldPos, float3 camPos, float roughness)
//{
//    float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
    
//    if (specExponent < 0.05f)
//    {
//        return 0;
//    }
    
//    float3 view = normalize(camPos - worldPos);
//    float3 reflection = reflect(-lightDirection, normal);
    
//    return pow(max(dot(reflection, view), 0.0f), specExponent);
//}

float Attenuate(Light light, float3 worldPos)
{
    float dist = distance(light.Position, worldPos);
    float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
    return att * att;
}

// Normal Distribution Function: GGX (Trowbridge-Reitz)
//
// a - Roughness
// h - Half vector
// n - Normal
// 
// D(h, n, a) = a^2 / pi * ((n dot h)^2 * (a^2 - 1) + 1)^2
float GGX(float3 n, float3 h, float roughness)
{
    float NdotH2 = pow(saturate(dot(n, h)), 2);
    float a2 = pow(roughness, 2);
    
    float denom = NdotH2 * (a2 - 1) + 1;
    
    return a2 / (PI * pow(denom, 2));
    
}

// Fresnel term - Schlick approx.
// 
// v - View vector
// h - Half vector
// f0 - Value when l = n
//
// F(v,h,f0) = f0 + (1-f0)(1 - (v dot h))^5
float3 Schlick(float3 v, float3 h, float3 f0)
{
    float VdotH = saturate(dot(v, h));
    
    return f0 + (1 - f0) * pow(1 - VdotH, 5);
}

// Geometric Shadowing - Schlick-GGX
// - k is remapped to a / 2, roughness remapped to (r+1)/2 before squaring!
//
// n - Normal
// v - View vector
//
// G_Schlick(n,v,a) = (n dot v) / ((n dot v) * (1 - k) * k)
//
// Full G(n,v,l,a) term = G_SchlickGGX(n,v,a) * G_SchlickGGX(n,l,a)
float SchlickGGX(float3 n, float3 v, float roughness)
{
    float k = pow(roughness + 1, 2) / 8.0f;
    float NdotV = saturate(dot(n, v));
    
    return 1 / (NdotV * (1 - k) + k);

}

// Cook-Torrance Microfacet BRDF (Specular)
//
// f(l,v) = D(h)F(v,h)G(l,v,h) / 4(n dot l)(n dot v)
// - parts of the denominator are canceled out by numerator (see below)
//
// D() - Normal Distribution Function - Trowbridge-Reitz (GGX)
// F() - Fresnel - Schlick approx
// G() - Geometric Shadowing - Schlick-GGX
float3 MicrofacetBRDF(float3 n, float3 l, float3 v, float roughness, float3 f0, out float3 F_out)
{
    float3 h = normalize(v + l);
    
    float D = GGX(n, h, roughness);
    float3 F = Schlick(v, h, f0);
    float G = SchlickGGX(n, v, roughness) * SchlickGGX(n, l, roughness);
    
    F_out = F;
    
    float3 specularResult = (D * F * G) / 4;
    
    return specularResult * max(dot(n, l), 0);
}

float3 DiffuseEnergyConserve(float3 diffuse, float3 specular, float metalness)
{
    return diffuse * (1 - specular) * (1 - metalness);
}

float3 DirectionalLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float metalness, float3 surfaceColor, float3 specularColor)
{
    float3 lightDirection = normalize(-light.Direction);
    float3 camDirection = normalize(camPos - worldPos);
    
    float3 F;
    
    float diffusion = Diffuse(normal, lightDirection);
    float3 specular = MicrofacetBRDF(normal, lightDirection, camDirection, roughness, specularColor, F);
    
    float3 balanceDiff = DiffuseEnergyConserve(diffusion, F, metalness);

    return (balanceDiff * surfaceColor + specular) * light.Intensity * light.Color;
}

float3 PointLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float metalness, float3 surfaceColor, float3 specularColor)
{
    float3 lightDirection = normalize(light.Position - worldPos);
    float3 camDirection = normalize(camPos - worldPos);
    
    float3 F;
    
    float diffusion = Diffuse(normal, lightDirection);
    float3 specular = MicrofacetBRDF(normal, lightDirection, camDirection, roughness, specularColor, F);
    float attenuate = Attenuate(light, worldPos);
    
    float3 balanceDiff = DiffuseEnergyConserve(diffusion, F, metalness);
    
    return (balanceDiff * surfaceColor + specular) * attenuate * light.Intensity * light.Color;
}

float3 SpotLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float metalness, float3 surfaceColor, float3 specularColor)
{
    float3 lightDirection = normalize(light.Position - worldPos);
    float3 camDirection = normalize(camPos - worldPos);
    
    float3 F;
    
    float angle = dot(-lightDirection, normalize(light.Direction));

    float outer = cos(light.SpotOuterAngle);
    float inner = cos(light.SpotInnerAngle);
    float spotTerm = saturate((angle - outer) / (inner - outer));

    float diffusion = Diffuse(normal, lightDirection);
    float3 specular = MicrofacetBRDF(normal, lightDirection, camDirection, roughness, specularColor, F);
    float attenuation = Attenuate(light, worldPos);
    
    float3 balanceDiff = DiffuseEnergyConserve(diffusion, F, metalness);

    return (balanceDiff * surfaceColor + specular) * attenuation * spotTerm * light.Intensity * light.Color;
}

float3 NormalMapping(Texture2D nMap, SamplerState samp, float2 uv, float3 normal, float3 tangent)
{
    float3 normalFromMap = nMap.Sample(samp, uv).rgb * 2.0f - 1.0f;
    
    float3 n = normal;
    float3 t = normalize(tangent - n * dot(tangent, n));
    float3 b = cross(t, n);
    
    float3x3 TBN = float3x3(t, b, n);
    
    return normalize(mul(normalFromMap, TBN));
}
#endif