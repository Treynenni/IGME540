#pragma once

#include "Input.h"
#include "Transformation.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;
using namespace std;

class Camera
{
public:
	Camera(float aspectRatio, XMFLOAT3 initPosition, float inFov, float inMoveSpeed, float inMouseSpeed);

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();

	float GetFOV();

	shared_ptr<Transformation> GetTransform();


	void UpdateProjectionMatrix(float aspectRatio);
	void UpdateViewMatrix();

	void Update(float dt);

	

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	Transformation transform;

	float fov;

	float moveSpeed;
	float mouseSpeed;

};

