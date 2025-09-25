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
	XMFLOAT3& GetPosition();
	XMFLOAT3& GetRotation();
	XMFLOAT3& GetScale();

	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT4X4 GetWorldInverseTransposeMatrix();

	XMFLOAT3& GetRight();
	XMFLOAT3& GetUp();
	XMFLOAT3& GetForward();

	void UpdateWorldMatrix();

	void UpdateVectors();

	// Transformer Methods
	void MoveAbsolute(float x, float y, float z);
	void MoveAbsolute(XMFLOAT3 position);

	void MoveRelative(float x, float y, float z);
	void MoveRelative(XMFLOAT3 offset);

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

	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT3 forward;


};