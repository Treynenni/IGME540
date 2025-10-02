#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// --------------------------------------------------------
// A custom constant buffer struct
// --------------------------------------------------------
struct BufferStructs
{
	XMFLOAT4 colorTint;        // Overall tint
	XMFLOAT4X4 world;	       // Transformations
	XMFLOAT4X4 projection;
	XMFLOAT4X4 view;
};