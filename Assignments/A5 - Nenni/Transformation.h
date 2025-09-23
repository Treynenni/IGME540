#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Transformation
{

public:
	// Constructor
	Transformation();

	// Setter Methods
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 position);

	void SetRotation(float p, float y, float r);
	void SetRotation(XMFLOAT3 rotation);

	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);

	// Getter Methods
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();

	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT4X4 GetWorldInverseTransposeMatrix();

	void UpdateWorldMatrix();

	// Transformer Methods
	void MoveAbsolute(float x, float y, float z);
	void MoveAbsolute(XMFLOAT3 position);

	void Rotation(float p, float y, float r);
	void Rotation(XMFLOAT3 rotation);

	void Scale(float x, float y, float z);
	void Scale(XMFLOAT3 scale);

private:

	// Vector Variables
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	// World Matrix
	XMFLOAT4X4 world;
	XMFLOAT4X4 worldInverseTranspose;


};