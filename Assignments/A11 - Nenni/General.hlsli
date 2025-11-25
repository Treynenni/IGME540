#ifndef __GGP_SHADER_INCLUDES__ // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES__

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition	: SV_POSITION;
    float2 uv				: TEXCOORD;
    float3 normal			: NORMAL;
    float3 worldPosition	: POSITION;
    float3 tangent          : TANGENT;
};

struct VertexToPixel_Sky
{
    // Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition	: SV_POSITION;
    float3 sampleDir		: DIRECTION;
};

struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float3 localPosition	: POSITION; // XYZ position
    float2 uv				: TEXCOORD;
    float3 normal			: NORMAL;
    float3 tangent			: TANGENT;
};

#endif