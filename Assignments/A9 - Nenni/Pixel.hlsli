#ifndef __GGP_PIXELSHADER_INCLUDES__ // Each .hlsli file needs a unique identifier!
#define __GGP_PIXELSHADER_INCLUDES__

cbuffer ExternalData : register(b0)
{
    float4 color;
    float2 uvScale;
    float2 uvOffset;
    float3 camPosition;
    float roughness;
    float3 ambientColor;
    float time;
}

#endif