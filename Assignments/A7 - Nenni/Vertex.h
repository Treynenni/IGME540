#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	XMFLOAT3 Position;	    // The local position of the vertex
	XMFLOAT2 UV;           
	XMFLOAT3 Normal;
};