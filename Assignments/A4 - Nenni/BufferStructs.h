#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom constant buffer struct
// --------------------------------------------------------
struct BufferStructs
{
	DirectX::XMFLOAT4 colorTint;        // Overall tint
	DirectX::XMFLOAT3 offset;	   // Offset of tint
	
};