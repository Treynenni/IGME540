#pragma once

#include "Input.h"
#include "Transformation.h"
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(float aspectRatio, XMFLOAT3 initPosition, float inFov, float inMoveSpeed, float inMouseSpeed);

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();

	void UpdateProjectionMatrix(float aspectRatio);
	void UpdateViewMatrix();

	void Update(float dt);

private:
	Transformation transform;

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	float fov;
	float moveSpeed;
	float mouseSpeed;

};

