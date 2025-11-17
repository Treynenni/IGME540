#include "Transformation.h"

Transformation::Transformation()
{
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);

	XMStoreFloat4x4(&world, XMMatrixIdentity());
	XMStoreFloat4x4(&worldInverseTranspose, XMMatrixIdentity());

}

void Transformation::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transformation::SetPosition(XMFLOAT3 newPosition)
{
	position = newPosition;
}

void Transformation::SetRotation(float p, float y, float r)
{
	rotation.x = p;
	rotation.y = y;
	rotation.z = r;

	UpdateVectors();
}

void Transformation::SetRotation(XMFLOAT3 newRotation)
{
	rotation = newRotation;

	UpdateVectors();
}

void Transformation::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Transformation::SetScale(XMFLOAT3 newScale)
{
	scale = newScale;
}

XMFLOAT3& Transformation::GetPosition()
{
	return position;
}

XMFLOAT3& Transformation::GetRotation()
{
	return rotation;
}

XMFLOAT3& Transformation::GetScale()
{
	return scale;
}

XMFLOAT4X4 Transformation::GetWorldMatrix()
{
	UpdateWorldMatrix();

	return world;
}

XMFLOAT4X4 Transformation::GetWorldInverseTransposeMatrix()
{
	UpdateWorldMatrix();
	
	return worldInverseTranspose;
}

XMFLOAT3& Transformation::GetRight()
{
	return right;
}

XMFLOAT3& Transformation::GetUp()
{
	return up;
}

XMFLOAT3& Transformation::GetForward()
{
	return forward;
}

void Transformation::UpdateWorldMatrix() 
{
	XMMATRIX worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&world, worldMatrix);
	XMStoreFloat4x4(&worldInverseTranspose, XMMatrixInverse(0, XMMatrixTranspose(worldMatrix)));
}

void Transformation::UpdateVectors()
{
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMStoreFloat3(&right, XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotQuat));
	XMStoreFloat3(&up, XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotQuat));
	XMStoreFloat3(&forward, XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotQuat));
	float b = 0;
}

void Transformation::MoveAbsolute(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Transformation::MoveAbsolute(XMFLOAT3 addPosition)
{
	position.x += addPosition.x;
	position.y += addPosition.y;
	position.z += addPosition.z;
}

void Transformation::MoveRelative(float x, float y, float z)
{
	XMVECTOR direction = XMVECTOR{ x, y, z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMFLOAT3 add; 
	XMStoreFloat3(&add, XMVector3Rotate(direction, rotate));

	position.x += add.x;
	position.y += add.y;
	position.z += add.z;
}

void Transformation::MoveRelative(XMFLOAT3 offset)
{
	XMVECTOR direction = XMLoadFloat3(&offset);
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMFLOAT3 add;
	XMStoreFloat3(&add, XMVector3Rotate(direction, rotate));

	position.x += add.x;
	position.y += add.y;
	position.z += add.z;
}

void Transformation::Rotation(float p, float y, float r)
{
	rotation.x += p;
	rotation.y += y;
	rotation.z += r;

	UpdateVectors();
}

void Transformation::Rotation(XMFLOAT3 addRotation)
{
	rotation.x += addRotation.x;
	rotation.y += addRotation.y;
	rotation.z += addRotation.z;

	UpdateVectors();
}

void Transformation::Scale(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void Transformation::Scale(XMFLOAT3 multiplyScale)
{
	scale.x *= multiplyScale.x;
	scale.y *= multiplyScale.y;
	scale.z *= multiplyScale.z;
}
