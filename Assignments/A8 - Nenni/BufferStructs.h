#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// --------------------------------------------------------
// A custom constant buffer struct
// --------------------------------------------------------
struct VertexBufferData
{
	XMFLOAT4X4 world;	       // Transformations
	XMFLOAT4X4 projection;
	XMFLOAT4X4 view;
};

struct PixelBufferData 
{
	XMFLOAT4 colorTint;        // Overall tint
	float time;
	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;
};