#include "Material.h"
#include "Graphics.h"

Material::Material(XMFLOAT4 inTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader)
{
	SetTint(inTint);
	SetVertexShader(inVertexShader);
	SetPixelShader(inPixelShader);
}

void Material::SetTint(XMFLOAT4 inTint)
{
	tint = inTint;
}

void Material::SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader)
{
	vertexShader = inVertexShader;
}

void Material::SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> inPixelShader)
{
	pixelShader = inPixelShader;
}

XMFLOAT4 Material::GetTint()
{
	return tint;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

void Material::AddTextureSRV(unsigned int index, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	textureSRVs.insert({ index, srv });
}

void Material::AddSampler(unsigned int index, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	samplers.insert({ index, sampler });
}

void Material::BindTextureAndSampler()
{
	for (auto& t : textureSRVs) { Graphics::Context->PSSetShaderResources(t.first, 1, t.second.GetAddressOf()); }
	for (auto& s : samplers) { Graphics::Context->PSSetSamplers(s.first, 1, s.second.GetAddressOf()); }
}
