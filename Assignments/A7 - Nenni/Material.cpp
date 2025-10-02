#include "Material.h"

Material::Material(XMFLOAT4 inTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> inVertexShader, Microsoft::WRL::ComPtr<ID3D11VertexShader> inPixelShader)
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

void Material::SetPixelShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> inPixelShader)
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
