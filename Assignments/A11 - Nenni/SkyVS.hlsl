#include "General.hlsli"

cbuffer ExternalData : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

VertexToPixel_Sky main(VertexShaderInput input)
{
    VertexToPixel_Sky output;
    
    float4x4 viewNoTranslation = view;
    
    viewNoTranslation._14 = 0;
    viewNoTranslation._24 = 0;
    viewNoTranslation._34 = 0;
    
    float4x4 vp = mul(projection, viewNoTranslation);
    output.screenPosition = mul(vp, float4(input.localPosition, 1.0f));
    
    output.screenPosition.z = output.screenPosition.w;
    
    output.sampleDir = input.localPosition;
    
    return output;

}


