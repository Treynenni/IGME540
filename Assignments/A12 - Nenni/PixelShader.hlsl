#include "Pixel.hlsli"

// Texture Resources
Texture2D Albedo                        : register(t0);
Texture2D NormalMap                     : register(t1);
Texture2D RoughnessMap                  : register(t2);
Texture2D MetalnessMap                  : register(t3);
Texture2D ShadowMap                     : register(t4);

SamplerState BasicSampler               : register(s0);
SamplerComparisonState ShadowSampler    : register(s1);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    // Perform the perspective divide (divide by W) ourselves
    input.shadowMapPos /= input.shadowMapPos.w;
    
    // Convert the normalized device coordinates to UVs for sampling
    float2 shadowUV = input.shadowMapPos.xy * 0.5f + 0.5f;
    shadowUV.y = 1 - shadowUV.y; // Flip the Y
    
    // Grab the distances we need: light-to-pixel and closest-surface
    float distToLight = input.shadowMapPos.z;
    
    // Get a ratio of comparison results using SampleCmpLevelZero()
    float shadowAmount = ShadowMap.SampleCmpLevelZero(
        ShadowSampler,
        shadowUV,
        distToLight).r;
    
    // Adjust uv
    input.uv = input.uv * uvScale + uvOffset;
    
    // Normalize normal and tangent
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    
    // Apply normal map
    input.normal = NormalMapping(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);
    
    // Sample roughness
    float roughness = RoughnessMap.Sample(BasicSampler, input.uv).r;
    
    // Sample metal
    float metal = MetalnessMap.Sample(BasicSampler, input.uv).r;
    
    // Establish surface color
    float3 surfaceColor = Albedo.Sample(BasicSampler, input.uv).rgb * color.rgb;
    surfaceColor = pow(surfaceColor, 2.2f);
    
    // Establish specular color
    float3 specularColor = lerp(0.04f, surfaceColor.rgb, metal);
    
    // Ambient lighting
    float3 totalLight = 0;
    
    // Additional lighting
    for (int i = 0; i < 1; i++)
    {
        Light light = lights[i];
        light.Direction = normalize(light.Direction);
        float3 lightResult = 0;
        
        switch (light.Type)
        { 
            case LIGHT_TYPE_DIRECTIONAL:
                lightResult = DirectionalLight(light, input.normal, input.worldPosition, camPosition, roughness, metal, surfaceColor, specularColor);
                if (i == 0)
                {
                    lightResult *= shadowAmount;
                }
                totalLight += lightResult;
                break;
            
            case LIGHT_TYPE_POINT:
                totalLight += PointLight(light, input.normal,  input.worldPosition, camPosition, roughness, metal, surfaceColor, specularColor);
                break;
            
            case LIGHT_TYPE_SPOT:
                totalLight += SpotLight(light, input.normal, input.worldPosition, camPosition, roughness, metal, surfaceColor, specularColor);
                break;
        }
    }   
    
    totalLight = pow(totalLight, 1.0f / 2.2f);
    
    return float4(totalLight, 1.0f);
}