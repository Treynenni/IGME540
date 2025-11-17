#include "Pixel.hlsli"

// Texture Resources
Texture2D SurfaceTexture    : register(t0);
Texture2D NormalMap         : register(t1);

SamplerState BasicSampler   : register(s0);

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
    // Adjust uv
    input.uv = input.uv * uvScale + uvOffset;
    
    // Normalize normal and tangent
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    
    // Apply normal map
    input.normal = NormalMapping(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);
    
    // Establish surface color
    float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb * color.rgb;
    
    // Ambient lighting
    float3 totalLight = ambientColor * surfaceColor;
    
    // Additional lighting
    for (int i = 0; i < 5; i++)
    {
        Light light = lights[i];
        light.Direction = normalize(light.Direction);
        
        switch (light.Type)
        { 
            case LIGHT_TYPE_DIRECTIONAL:
                totalLight += DirectionalLight(light, input.normal, input.worldPosition, camPosition, roughness, surfaceColor);
                break;
            
            case LIGHT_TYPE_POINT:
                totalLight += PointLight(light, input.normal,  input.worldPosition, camPosition, roughness, surfaceColor);
                break;
            
            case LIGHT_TYPE_SPOT:
                totalLight += SpotLight(light, input.normal, input.worldPosition, camPosition, roughness, surfaceColor);
                break;
        }
    }   
    
    return float4(totalLight, 1.0);
}