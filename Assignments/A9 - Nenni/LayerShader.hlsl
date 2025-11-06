#include "General.hlsli"
#include "Pixel.hlsli"

// Texture Resources
Texture2D Texture1 : register(t0);
Texture2D Texture2 : register(t1);

SamplerState BasicSampler : register(s0);

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
    
    float4 surfaceColor = Texture1.Sample(BasicSampler, input.uv).rgba + (Texture2.Sample(BasicSampler, input.uv).rgba * float4(0.4, 0.3, 0.2, 1.0));
    surfaceColor *= color;
    
    return float4(surfaceColor);
}