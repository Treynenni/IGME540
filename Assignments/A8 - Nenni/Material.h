#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <unordered_map>

using namespace DirectX;

class Material
{
public:
	Material(XMFLOAT4 inTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader);

	void SetTint(XMFLOAT4 inTint);
	void SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader);
	void SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader);
	void SetUVScale(XMFLOAT2 scale);
	void SetUVOffset(XMFLOAT2 offset);

	XMFLOAT4& GetTint();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	XMFLOAT2 GetUVScale();
	XMFLOAT2 GetUVOffset();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(unsigned int index);

	void AddTextureSRV(unsigned int index, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void AddSampler(unsigned int index, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	void BindTextureAndSampler();

private:
	XMFLOAT4 tint;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;

	std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureSRVs;
	std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers;
};

