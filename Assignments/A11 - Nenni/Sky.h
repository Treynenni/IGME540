#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <wrl/client.h>

using namespace std;

class Sky
{
public:
	Sky(const wchar_t* right, const wchar_t* left, const wchar_t* up, const wchar_t* down, const wchar_t* front, const wchar_t* back,
		shared_ptr<Mesh> mesh, Microsoft::WRL::ComPtr<ID3D11VertexShader> inSkyVS, Microsoft::WRL::ComPtr<ID3D11PixelShader> inSkyPS, Microsoft::WRL::ComPtr<ID3D11SamplerState> inSamplerOptions);

	~Sky();

	void Draw(shared_ptr<Camera> cam);

	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> GetSkyTexture();

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> skyPS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> skyVS;

	shared_ptr<Mesh> skyMesh;

	void InitRenderState();

// Helper for creating a cubemap from 6 individual textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

};

