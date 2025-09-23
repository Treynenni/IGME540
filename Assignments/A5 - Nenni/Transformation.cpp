#include "Transformation.h"

Transformation::Transformation()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

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
}

void Transformation::SetRotation(XMFLOAT3 newRotation)
{
	rotation = newRotation;
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

XMFLOAT3 Transformation::GetPosition()
{
	return position;
}

XMFLOAT3 Transformation::GetRotation()
{
	return rotation;
}

XMFLOAT3 Transformation::GetScale()
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

void Transformation::UpdateWorldMatrix() {
	XMMATRIX worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&world, worldMatrix);
	XMStoreFloat4x4(&worldInverseTranspose, XMMatrixInverse(0, XMMatrixTranspose(worldMatrix)));
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

void Transformation::Rotation(float p, float y, float r)
{
	rotation.x += p;
	rotation.y += y;
	rotation.z += r;
}

void Transformation::Rotation(XMFLOAT3 addRotation)
{
	rotation.x += addRotation.x;
	rotation.y += addRotation.y;
	rotation.z += addRotation.z;
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
