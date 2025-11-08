#ifndef __GGP_LIGHTS__ // Each .hlsli file needs a unique identifier!
#define __GGP_LIGHTS__

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



#endif