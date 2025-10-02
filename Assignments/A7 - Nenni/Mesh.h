#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "Graphics.h" 
#include "Vertex.h"


class Mesh
{
public:
	
	// Constructor/Destructor
	Mesh(unsigned int* indices, Vertex* vertices, int iCount, int vCount);
	Mesh(char* objFile);
	~Mesh();

	// Getter Methods
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();

	void Draw();


private:

	// Buffers to hold gameplay data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	int indexCount;
	int vertexCount;

	void CreateBuffers(Vertex* vertices, unsigned int* indices);
	void CreateBuffers(std::vector<Vertex> verts, std::vector<UINT> indices);

};

