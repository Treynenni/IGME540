#include "Pixel.hlsli"

// Texture Resources
Texture2D SurfaceTexture    : register(t0);

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
    input.uv = input.uv * uvScale + uvOffset;
    input.normal = normalize(input.normal);
    
    float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb * color.rgb;
    float3 totalLight = ambientColor * surfaceColor;
    
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
    
    //return float4(input.normal, 1);
    
    //return float4(dirLight1.Color, 1);

}