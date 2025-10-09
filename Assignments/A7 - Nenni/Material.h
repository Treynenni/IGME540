#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>

using namespace DirectX;

class Material
{
public:
	Material(XMFLOAT4 inTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader);

	void SetTint(XMFLOAT4 inTint);
	void SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader);
	void SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader);

	XMFLOAT4 GetTint();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();

private:
	XMFLOAT4 tint;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};

