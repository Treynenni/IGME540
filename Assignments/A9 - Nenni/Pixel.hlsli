#ifndef __GGP_PIXELSHADER_INCLUDES__ // Each .hlsli file needs a unique identifier!
#define __GGP_PIXELSHADER_INCLUDES__

#define MAX_SPECULAR_EXPONENT 256.0f

#include "General.hlsli"
#include "Light.hlsli"

cbuffer ExternalData : register(b0)
{
    float4 color;
    float2 uvScale;
    float2 uvOffset;
    float3 camPosition;
    float roughness;
    float3 ambientColor;
    float time;
    Light dirLight1;
}

#endif