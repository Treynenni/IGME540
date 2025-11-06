#pragma once

#include <DirectXMath.h>
#include "Lights.h"

using namespace DirectX;

// --------------------------------------------------------
// A custom constant buffer struct
// --------------------------------------------------------
struct VertexBufferData
{
	XMFLOAT4X4 world;	       // Transformations
	XMFLOAT4X4 projection;
	XMFLOAT4X4 view;
	XMFLOAT4X4 worldInvTranspose;
};

struct PixelBufferData 
{
	XMFLOAT4 colorTint;        // Overall tint
	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;
	XMFLOAT3 camPosition;
	float roughness;
	XMFLOAT3 ambientColor;
	float time;
	Light dirLight1;

};