#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	XMFLOAT3 Position;	    // The local position of the vertex
	XMFLOAT2 UV;            // UV Coords
	XMFLOAT3 Normal;		// Normal Vector of Vertex
	XMFLOAT3 Tangent;       // Tangent Vector of Vertex
};